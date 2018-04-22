//=========================================================================
/**
*  @file      Quadrics.cpp
*
*  项目描述： 二次曲面
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

#include "Quadrics.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	 
                                      


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							

/** 绘制模式 */
GLint renderMode[] = { GLU_FILL,GLU_LINE,GLU_SILHOUETTE };
GLuint mode = 0;  /**< 绘制模式索引 */

/** 用于输出信息 */
char *RenderMode[] = 
{
	"GLU_FILL",
	"GLU_LINE",
	"GLU_SILHOUETTE"
};

/** 定义光源的属性值 */
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 	/**< 环境光参数 */
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< 漫射光参数 */
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< 镜面光参数 */
GLfloat LightPosition[]= { 0.0f, 0.0f, 5.0f, 1.0f };	/**< 光源位置 */

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Quadrics * quad = new Quadrics(class_name);
	return reinterpret_cast<GLApplication *>(quad);
}


/** 构造函数 */
Quadrics::Quadrics(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	rot = 0.0;
	lp = FALSE;
	light = FALSE;
	
}

/** 装载纹理 */
bool Quadrics::LoadTexture()
{
	char filename[128] ;                                         /**< 用来保存文件名 */
	for(int i=0; i< 4; i++)
	{
		sprintf(filename,"data/image%d",i);
		strcat(filename,".bmp");
		if(!Texture[i].LoadBitmap(filename))                     /**< 载入位图文件 */
		{
			MessageBox(NULL,"装载位图文件失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
			return false;
		}
		glGenTextures(1, &Texture[i].ID);                        /**< 生成一个纹理对象名称 */
		 
		glBindTexture(GL_TEXTURE_2D, Texture[i].ID);             /**< 创建纹理对象 */
		/** 控制滤波 */
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
		/** 创建纹理 */
		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture[i].imageWidth,
						Texture[i].imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					    Texture[i].image);
	}
    
	return true;
}

/** 初始化OpenGL */
bool Quadrics::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);
	glEnable(GL_TEXTURE_2D);                       /**< 启用纹理映射 */
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	
	/** 载入纹理 */
	if( !LoadTexture() )
		MessageBox(NULL,"载入纹理错误!","错误",MB_OK);

    /** 设置光源的属性值 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);		/**< 设置环境光 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);		/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);	/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);	/**< 设置光源位置 */
	/** 启用光源 */
	glEnable(GL_LIGHT1); 
	/** 初始化字体 */
	if(!font.InitFont())
		MessageBox(NULL,"初始化字体失败！","错误",MB_OK);

	/**< 创建二次几何体 */
	quadratic = gluNewQuadric();							
	gluQuadricNormals(quadratic, GLU_SMOOTH);			/**< 使用平滑法线 */
	gluQuadricTexture(quadratic, GL_TRUE);				/**< 使用纹理 */
	
	ResizeDraw(true);									/**< 改变OpenGL窗口大小，直接调用子类的函数 */

  	return true;                                        /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Quadrics::Uninit()									
{
	for(int i=0; i < 4; i++) 
	{
		Texture[i].FreeImage();               /** 释放纹理图像占用的内存 */
	    glDeleteTextures(1, &Texture[i].ID);  /**< 删除纹理对象 */
	}
	/** 删除二次对象 */
	if(quadratic != NULL)
		gluDeleteQuadric(quadratic);
}

/** 程序更新函数 */
void Quadrics::Update(DWORD milliseconds)						
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

	if (m_Keys.IsPressed(VK_SPACE) && !sp)		  /**< L 键已按下并且松开了? */
	{
		sp = TRUE;						      /**< lp 设为 TRUE */
		mode += 1;
		if(mode > 2)
			mode = 0;
		
		gluQuadricDrawStyle(quadratic,renderMode[mode]);
		
	}

	if (!m_Keys.IsPressed(VK_SPACE))				  /**< L键松开了么? */
	{
		sp = FALSE;						      /**< 若是，则将lp设为FALSE */
	}
	
	/** 更新旋转角度 */
 	rot += milliseconds/20.0;
	    
}

/** 绘制函数 */
void Quadrics::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	  /**< 清除缓存 */		
	glLoadIdentity();	
    
	/** 绘制过程 */
	glTranslatef(0.0f,0.0f,-8.0f);
	///绘制圆柱体
	glBindTexture(GL_TEXTURE_2D, Texture[0].ID);                      /**< 绑定纹理 */
	glPushMatrix();
	  glTranslatef(-2.0f,1.1f,0.0f);
	  glRotatef(-90,1.0f,0.0f,0.0f);
      glRotatef(rot,1.0f,0.0f,1.0f);
      gluCylinder(quadratic,0.8f,0.8f,1.8f,32,32);
	glPopMatrix();
	///绘制圆锥体
	glBindTexture(GL_TEXTURE_2D, Texture[1].ID);                      /**< 绑定纹理 */
	glPushMatrix();
	  glTranslatef(2.0f,1.1f,0.0f);
	  glRotatef(-90,1.0f,0.0f,0.0f);
      glRotatef(rot,1.0f,0.0f,1.0f);
      gluCylinder(quadratic,1.0f,0.0f,1.8f,32,32);
    glPopMatrix();
    ///绘制圆盘	   
    glBindTexture(GL_TEXTURE_2D, Texture[2].ID);                      /**< 绑定纹理 */
	glPushMatrix();
	  glTranslatef(-2.0f,-1.1f,0.0f);
      glRotatef(rot,0.0f,1.0f,0.0f);
	  gluDisk(quadratic,0.4f,1.2f,32,32);
	glPopMatrix();
	///绘制球体  
	glBindTexture(GL_TEXTURE_2D, Texture[3].ID);                      /**< 绑定纹理 */
	glPushMatrix();
	  glTranslatef(2.0f,-1.1f,0.0f);
      glRotatef(rot,0.0f,1.0f,0.0f);
	  gluSphere(quadratic,1.1f,32,32);
	glPopMatrix();
	/** 输出一些信息 */
	char string[256];                                                /**< 保存输出信息 */
	sprintf(string,"当前模式：%s",RenderMode[mode]);
	font.PrintText(string,-5.0f,-4.0f);
	
	glFlush();				                                           /**< 强制执行所有的OpenGL命令 */
}
