//=========================================================================
/**
*  @file  Example.cpp
*
*  项目描述： 纹理映射
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-11-26
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

#include "Example.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	 
//#include <math.h>                                              /**< 包含math.h头文件 */                                             


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							

/** 定义光源的属性值 */
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 	/**< 环境光参数 */
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< 漫射光参数 */
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< 镜面光参数 */
GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };	/**< 光源位置 */

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Example * example = new Example(class_name);
	return reinterpret_cast<GLApplication *>(example);
}


/** 构造函数 */
Example::Example(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	rot = 0.0;
}


/** 初始化OpenGL */
bool Example::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */
	
	glEnable(GL_TEXTURE_2D);                          /**< 启用纹理映射 */
	if(!Texture.LoadBitmap("image.bmp"))              /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件失败！","错误",MB_OK);  /**< 如果载入失败则弹出对话框 */
		return false;
	}
	glGenTextures(1, &Texture.ID);        /**< 生成一个纹理对象名称 */
	 
	glBindTexture(GL_TEXTURE_2D, Texture.ID); /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    /** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, Texture.imageWidth,
	                  Texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
	                  Texture.image);
    /** 设置光源的属性值 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				/**< 设置环境光 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);			/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			/**< 设置光源位置 */
	/** 启用光照和光源 */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);     

  	return true;           /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Example::Uninit()									
{
    Texture.FreeImage();              /** 释放纹理图像占用的内存 */
	glDeleteTextures(1, &Texture.ID); /**< 删除纹理对象 */
}

/** 程序更新函数 */
void Example::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

	///if (m_Keys.IsPressed(VK_F1) == true)						/**< 按F1切换窗口/全屏模式 */
	///{
	///	ToggleFullscreen();										
	///}
 /** 用户自定义的更新过程 */
	rot += milliseconds/20.0;
	    
}

/** 绘制函数// */
void Example::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 /**< 清楚缓存 */		
	glLoadIdentity();	
    /** 绘制过程 */	
	glTranslatef(0.0f, 0.0f, -7.0f);
	glRotatef(rot,1.0f,1.0f,0.0f);
	/** 选择纹理 */
	glBindTexture(GL_TEXTURE_2D, Texture.ID);
	/** 开始绘制四边形 */
	glBegin(GL_QUADS);												
		/// 前侧面
		glNormal3f( 0.0f, 0.0f, 1.0f);								/**< 指定法线指向观察者 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		/// 后侧面
		glNormal3f( 0.0f, 0.0f,-1.0f);								/**< 指定法线背向观察者 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		/// 顶面
		glNormal3f( 0.0f, 1.0f, 0.0f);								/**< 指定法线向上 */
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		/// 底面
		glNormal3f( 0.0f,-1.0f, 0.0f);								/**< 指定法线朝下 */
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		/// 右侧面
		glNormal3f( 1.0f, 0.0f, 0.0f);								/**< 指定法线朝右 */
		glTexCoord2f(1.0f, 0.0f); glVertex3f( 1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f( 1.0f,  1.0f, -1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f( 1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 0.0f); glVertex3f( 1.0f, -1.0f,  1.0f);	
		/// 左侧面
		glNormal3f(-1.0f, 0.0f, 0.0f);								/**< 指定法线朝左 */
		glTexCoord2f(0.0f, 0.0f); glVertex3f(-1.0f, -1.0f, -1.0f);	
		glTexCoord2f(1.0f, 0.0f); glVertex3f(-1.0f, -1.0f,  1.0f);	
		glTexCoord2f(1.0f, 1.0f); glVertex3f(-1.0f,  1.0f,  1.0f);	
		glTexCoord2f(0.0f, 1.0f); glVertex3f(-1.0f,  1.0f, -1.0f);	
	glEnd();														/**< 四边形绘制结束 */
	
    glFlush();				           /**< 强制执行所有的OpenGL命令 */
}
