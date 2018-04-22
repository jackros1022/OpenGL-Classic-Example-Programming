//=========================================================================
/**
*  @file      Bezier.cpp
*
*  项目描述： Bezier曲面
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-04
*
*  在这个类中您必须重载如下几个虚函数
*																								
*	virtual bool Init();														
*		执行所有的初始化工作，如果成功函数返回true							
*																			
*	virtual void Uninit();													
*		执行所有的卸载工作										
*																			
*	virtual void Update(DWORD milliseconds);										
*		执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位
*																			
*	virtual void Draw();															
*		执行所有的绘制操作
*/
//=========================================================================

#include "Bezier.h"											   /**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	 
                                      

#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							


/** 绘制模式 */
GLint renderMode[] = { GL_FILL,GL_LINE };
GLuint mode = 0;  /**< 绘制模式索引 */

/** 用于输出信息 */
char *RenderMode[] = 
{
	"GL_FILL",
	"GL_LINE",
};

/** 定义光源的属性值 */
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 	/**< 环境光参数 */
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< 漫射光参数 */
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< 镜面光参数 */
GLfloat LightPosition[]= { 0.0f, 0.0f, 2.0f, 1.0f };	/**< 光源位置 */

/** 定义材质属性值 */
GLfloat mat_diffuse[] = { 0.4, 0.5, 0.6, 1.0 };         /**< 漫射光参数 */ 
GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };        /**< 镜面光参数 */
GLfloat mat_shinness[] = { 50.0 };                      /**< 镜面反射指数 */

/** 定义曲面的控制点 */
GLfloat ctrlpoints[4][4][3] = { 
	{{ -0.8f, -0.6f, 0.8f },{ -0.2f, -0.6f, 1.6f },{  0.2f, -0.6f, -0.4f },{ 0.6f, -0.6f,  0.8f }},
	{{ -0.6f, -0.2f, 0.8f },{ -0.2f, -0.2f, 1.6f },{  0.2f, -0.2f, -0.4f },{ 0.6f, -0.2f,  0.8f }},
	{{ -0.6f,  0.2f, 0.8f },{ -0.2f, -0.2f, 0.4f },{  0.2f,  0.2f,  0.0f },{ 0.3f,  0.2f, -0.4f }},
	{{ -0.6f,  0.6f, 0.8f },{ -0.2f,  0.6f, 0.4f },{ -0.8f,  0.6f,  0.0f },{ 0.8f,  0.6f, -0.4f }}
};

GLfloat texpts[2][2][2] = {{{0.0, 0.0}, {0.0, 1.0}}, {{1.0, 0.0}, {1.0, 1.0}}};

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Bezier * bezier = new Bezier(class_name);
	return reinterpret_cast<GLApplication *>(bezier);
}


/** 构造函数 */
Bezier::Bezier(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	rot = 0.0;
	light = FALSE;
	lp = FALSE;
	sp = FALSE;
	
}

/** 装载纹理 */
bool Bezier::LoadTexture()
{
	/** 载入位图文件 */
	if(!Texture.LoadBitmap("image.bmp"))                     /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
		return false;
	}
	glGenTextures(1, &Texture.ID);                           /**< 生成一个纹理对象名称 */
		
	glBindTexture(GL_TEXTURE_2D, Texture.ID);                /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture.imageWidth,
					Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					Texture.image);
    
	return true;
}
/** 初始化OpenGL */
bool Bezier::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);	       
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			

	/** 载入纹理 */
	if( !LoadTexture() )
		MessageBox(NULL,"载入纹理错误!","错误",MB_OK);
	
	/** 初始化字体 */
	if(!font.InitFont())
	{
		MessageBox(NULL,"初始化字体失败!","错误",MB_OK);
	}
	/** 启用纹理映射 */
	glEnable(GL_TEXTURE_2D);            
	
    /** 设置光源的属性值 */
	glLightfv(GL_LIGHT1, GL_AMBIENT,  LightAmbient);		/**< 设置环境光 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE,  LightDiffuse);		/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);	    /**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	    /**< 设置光源位置 */
	
    /** 设置材质属性值 */
	glMaterialfv(GL_FRONT, GL_DIFFUSE,   mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR,  mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shinness);
    
	/** 启用光源 */
	glEnable(GL_LIGHT1);

	/**   */
	glEnable(GL_MAP2_VERTEX_3); 
	glEnable(GL_MAP2_TEXTURE_COORD_2);
	glEnable(GL_AUTO_NORMAL);
	
	glMap2f(GL_MAP2_VERTEX_3, 0, 1, 3, 4, 0, 1, 12, 4,&ctrlpoints[0][0][0]); 
	glMap2f(GL_MAP2_TEXTURE_COORD_2, 0, 1, 2, 2, 0, 1, 4, 2, &texpts[0][0][0]);
    glMapGrid2f(20, 0.0, 1.0, 20, 0.0, 1.0); 
  
	ResizeDraw(true);									/**< 改变OpenGL窗口大小，直接调用子类的函数 */

  	return true;                                        /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Bezier::Uninit()									
{
	Texture.FreeImage();               /** 释放纹理图像占用的内存 */
	glDeleteTextures(1, &Texture.ID);  /**< 删除纹理对象 */
}

/** 程序更新函数 */
void Bezier::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)  /**< 按ESC退出 */
	{
		TerminateApplication();									
	}
/** 用户自定义的更新过程 */
	if (m_Keys.IsPressed('L') && !lp)		  /**< L 键已按下并且松开了? */
	{
		lp = TRUE;						      /**< lp 设为 TRUE */
		light = !light;					      /**< 切换光源的 TRUE/FALSE */
		if (!light)						      /**< 如果没有光源 */
		{
			glDisable(GL_LIGHTING);		      /**< 禁用光源 */
		}
		else							
		{
			glEnable(GL_LIGHTING);		      /**< 启用光源 */
		}
	}

	if (!m_Keys.IsPressed('L'))				  /**< L键松开了么? */
	{
		lp = FALSE;						      /**< 若是，则将lp设为FALSE */
	}

	/**   */
	if(m_Keys.IsPressed(VK_SPACE) && !sp)
	{
		sp = TRUE;
		mode += 1;
		if(mode > 1)
			mode = 0;
		glEvalMesh2(renderMode[mode],0,20,0,20);

	}
	if(!m_Keys.IsPressed(VK_SPACE))
	{
		sp = FALSE;
	}

	
	/** 更新旋转角度 */
 	rot += milliseconds/20.0;
	    
}

/** 绘制函数 */
void Bezier::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	  /**< 清除缓存 */		
	glLoadIdentity();	
    
	/** 绘制过程 */
	glTranslatef(0.0f,0.0f,-13.0f);
	glRotatef(rot,1.0,1.0,1.0);
	glScalef(3.0f,3.0f,3.0f);
	glEvalMesh2(renderMode[mode],0,20,0,20);

	char string[128];
	sprintf(string,"当前模式:%s",RenderMode[mode]);
    font.PrintText(string,-5.0f,3.0f);
    
	glFlush();				                                           /**< 强制执行所有的OpenGL命令 */
}
