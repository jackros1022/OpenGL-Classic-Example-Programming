//=========================================================================
/**
*  @file  BasciGraphics.cpp
*
*  项目描述： OpenGL基本图形的绘制
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-10-13	
*  修改日期： 2006-11-10
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

#include <cmath>                                                /**< 包含math头文件 */
#include "BasicGraphics.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>											


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							


/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	BasicGraphics * basic = new BasicGraphics(class_name);
	return reinterpret_cast<GLApplication *>(basic);
}


/** 构造函数 */
BasicGraphics::BasicGraphics(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	m_Angle = 0.0f;												/**< 设置初始角度为0 */
	
}

/** 初始化OpenGL */
bool BasicGraphics::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */

	return true;												/**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void BasicGraphics::Uninit()									
{
/** 用户自定义的卸载过程 */
///......
///......
}

/** 程序更新函数 */
void BasicGraphics::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

	if (m_Keys.IsPressed(VK_F1) == true)						/**< 按F1切换窗口/全屏模式 */
	{
		ToggleFullscreen();										
	}
/** 用户自定义的更新过程 */
	m_Angle += (float)(milliseconds) /15.0f;					/**< 更新旋转的角度 */

	if(m_Angle > 360.0f)
		m_Angle = 0.0f;
	

     
}

/** 绘制函数 */
void BasicGraphics::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/**< 清除颜色缓存和深度缓存 */		
	glLoadIdentity();
	/** 画线 */
	glPushMatrix();
		glTranslatef(-1.5f, 1.0f, -6.0f);
		glScalef(0.75f, 0.75f, 0.75f);
		glRotatef(m_Angle, 1.0f, 1.0f, 1.0f);
		glColor3f(1.0f, 0.0f, 0.0f);							
		glBegin(GL_LINE_LOOP); /**< 绘制闭合线段 */
			glVertex3f( 0.0f, 1.0f,-0.0f); /**< 第一个点a */ 
			glVertex3f(-1.0f,-1.0f, 0.0f); /**< 第二个点b */ 
			glVertex3f( 1.0f,-1.0f, 0.0f); /**< 第三个点c */ 
		glEnd();
	glPopMatrix();

	/** 画圆 */
	glPushMatrix();	
		glTranslatef(1.5f, 1.0f, -6.0f);
		glScalef(0.75f,0.75f,0.75f);
		glRotatef(m_Angle,1.0f,1.0f,1.0f);
		glColor3f(1.0f, 0.0f, 1.0f);
		glBegin(GL_TRIANGLE_FAN); /**< 绘制三角形扇 */
			glVertex3f(0,0,0.0f );   
			for ( int i = 0; i <= 370; i += 10)
			{
				float p = (float)(i * 3.14 / 180);
				glVertex3f((float)sin(p), (float)cos(p), 0.0f );
			}
		glEnd();
	glPopMatrix();
	/** 画三角形 */
	glPushMatrix();	
		glTranslatef(-1.5f, -1.0f, -6.0f);
		glScalef(0.75f,0.75f,0.75f);
		glRotatef(m_Angle,1.0f,1.0f,1.0f);
		glColor3f(0.0f, 1.0f, 1.0f);							
		glBegin(GL_TRIANGLES);	/**< 绘制三角形 */							
			glVertex3f( 0.0f,  1.0f, 0.0f);
			glVertex3f(-1.0f, -1.0f, 0.0f);
			glVertex3f( 1.0f, -1.0f, 0.0f);
		glEnd();
    glPopMatrix();
	/** 画正方形 */
	glPushMatrix();
		glTranslatef(1.5f,-1.0f,-6.0f);
		glScalef(0.75f,0.75f,0.75f);
		glRotatef(m_Angle,1.0f,1.0f,1.0f);
		glColor3f(1.f, 1.f, 0.f);
		glBegin(GL_QUADS);	/**< 绘制四边形 */
			glVertex3f(-1.0f, 1.0f, 0.0f);	/**< 左上 */
			glVertex3f( 1.0f, 1.0f, 0.0f);	/**< 右上 */				
			glVertex3f( 1.0f,-1.0f, 0.0f);	/**< 左下 */				
			glVertex3f(-1.0f,-1.0f, 0.0f);	/**< 右下 */				
		glEnd();					
	 glPopMatrix(); 

    glFlush();													/**< 强制执行所有的OpenGL命令 */
}
