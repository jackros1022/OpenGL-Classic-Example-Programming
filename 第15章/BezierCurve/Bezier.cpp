//=========================================================================
/**
*  @file      Bezier.cpp
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

#include "Bezier.h"											    /**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	 
                                      
#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							

/** 控制点数组 */
GLfloat points[4][3] = {
	   { -4.0f,  -2.0f,  0.0f }, { -2.0f,  3.0f,  2.0f},
       {  2.0f,  -3.0f, -2.0f }, {  4.0f,  2.0f,  0.0f}};


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
	
}


/** 初始化OpenGL */
bool Bezier::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
    
	/**启用并定义一维求值程序  */
	glEnable(GL_MAP1_VERTEX_3);
	glMap1f(GL_MAP1_VERTEX_3, 0.0, 1.0, 3, 4, &points[0][0]);
 	
	ResizeDraw(true);									/**< 改变OpenGL窗口大小，直接调用子类的函数 */

  	return true;                                        /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Bezier::Uninit()									
{
	
}

/** 程序更新函数 */
void Bezier::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)  /**< 按ESC退出 */
	{
		TerminateApplication();									
	}
/** 用户自定义的更新过程 */
	    
}

/** 绘制函数 */
void Bezier::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	  /**< 清除缓存 */		
	glLoadIdentity();	
    
	/** 绘制过程 */
	glTranslatef(0.0f,0.0f,-13.0f);
	glBegin(GL_LINE_STRIP);
	for( int i = 0; i < 100; i++)
	{
		/** 执行求值程序 */
		glEvalCoord1f((float)i / 100.0f); 
	}
	glEnd();
    
	/** 绘制控制点 */
	glPointSize(4.0);            /**< 设置点的大小 */
    
	glBegin(GL_POINTS);
	for(int j = 0; j < 4; j++)
		glVertex3fv(&points[j][0]);
    glEnd();

	glFlush();				      /**< 强制执行所有的OpenGL命令 */
}
