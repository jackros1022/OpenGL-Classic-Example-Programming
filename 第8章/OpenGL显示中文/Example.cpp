//=========================================================================
/**
*  @file  Example.cpp
*
*  项目描述： OpenGL显示中文
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-11-24
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
#include <math.h>                                              /**< 包含math.h头文件 */                                             


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							


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
	///初始化字体
	if(!font.InitFont())
	   return false;
  	return true;										/**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Example::Uninit()									
{
/** 用户自定义的卸载过程 */
///......
///......
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
	rot += milliseconds/5.0;
	    
}

/** 绘制函数// */
void Example::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 /**< 清楚缓存 */		
	glLoadIdentity();	
    /** 绘制过程 */	
	glColor3f(1.0f,0.0f,1.0f);                 /**< 指定颜色 */
	font.PrintText("OpenGL显示中文",-3.0f,2.0f); /**<　在指定位置输出字符串 */
	/** 输出立体汉字 */
	glColor3f(0.5f,1.0f,0.5f);       /**< 指定颜色 */
	glTranslatef(-3.0f,0.0f,-10.0f); /**< 指定位置 */
	glRotatef(rot,1.0,0.0,0.0);      /**< 旋转 */
	font.Print3DText("立体中文",0.2f); /**< 输出,厚度为0.2 */
		
    glFlush();				           /**< 强制执行所有的OpenGL命令 */
}
