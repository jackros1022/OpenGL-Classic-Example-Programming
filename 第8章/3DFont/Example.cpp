//=========================================================================
/**
*  @file  Example.cpp
*
*  项目描述： OpenGL3D字体演示
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-11-23
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

/** 定义光源的属性 */
///光源1
GLfloat ambient[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat diffuse[] = {0.0f,1.0f,1.0f,1.0f};
GLfloat specular[] = {0.0f,1.0f,1.0f,1.0f};
GLfloat position[] = {-1.0f,2.0f,-6.0f,1.0f};
/** 光源2 */
GLfloat ambient1[] = {1.0f,1.0f,1.0f,1.0f};
GLfloat diffuse1[] = {0.5f,0.0f,0.5f,1.0f};
GLfloat specular1[] = {0.5f,0.0f,0.5f,1.0f};
GLfloat position1[] = {3.0f,0.0f,-1.0f,1.0f};

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
	cnt1 = 0.0;
	cnt2 = 0.0;
	rot = 0.0;
}

/** 初始化OpenGL */
bool Example::Init()									
{
/** 用户自定义的初始化过程 */
    /** 设置光源的属性值 */
	glLightfv(GL_LIGHT1,GL_AMBIENT,ambient);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,diffuse);
	glLightfv(GL_LIGHT1,GL_SPECULAR,specular);
	//glLightfv(GL_LIGHT1,GL_POSITION,position);

	glLightfv(GL_LIGHT2,GL_AMBIENT,ambient1);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,diffuse1);
	glLightfv(GL_LIGHT2,GL_SPECULAR,specular1);
	glLightfv(GL_LIGHT2,GL_POSITION,position1);
	/** 开启光源 */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);
	glEnable(GL_LIGHT2);

	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */
	///初始化字体
	if(!font.InitFont(wglGetCurrentDC(),"Arial",60)) 
	   return false;
    
	return true;												/**< 成功返回 */
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

	cnt1 += milliseconds/1500.0;  /**< 更新计数器值 */
	cnt2 += milliseconds/1000.0;  /**< 更新计数器值 */
	rot += milliseconds/50.0;
	    
}

/** 绘制函数 */
void Example::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 /**< 清楚缓存 */		
	glLoadIdentity();	
    ///绘制字符过程
	glTranslatef(0.0f, 0.0f, -10.0f); /**< 移入屏幕 */
	glRotatef(rot,1.0f,0.0f,0.0f);						/**< 沿X轴旋转 */
	glRotatef(rot*1.5f,0.0f,1.0f,0.0f);					/**< 沿Y轴旋转 */
    glLightfv(GL_LIGHT1,GL_POSITION,position);          /**< 指定光源位置，随视点移动 */
	glRotatef(rot*1.4f,0.0f,0.0f,1.0f);					/**< 沿Z轴旋转 */
	glScalef(1.0f,2.0f,1.0f);
	glColor3f(cos(cnt1),sin(cnt2),1.0f - 0.5f * cos(cnt1 + cnt2)); /**< 指定颜色 */
    font.PrintText("Hello,OpenGL!",-3.5f + 2.0 * cos(cnt1), 3.5f * sin(cnt2)); /**< 调用方法输出字符串 */
	
    glFlush();													/**< 强制执行所有的OpenGL命令 */
}
