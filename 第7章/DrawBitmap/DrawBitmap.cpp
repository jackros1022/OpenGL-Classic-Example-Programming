//=========================================================================
/**
*  @file  DrawBitmap.cpp
*
*  项目描述： OpenGL绘制位图
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-11-20
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

#include "DrawBitmap.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>											


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							


/** 分别定义字符O P E N G L的位图数据 */ 
static GLubyte O_Rasters[13] = { 0x00,0x00, 0x7e,0xe7, 0xc3,0xc3, 0xc3,0xc3, 0xc3,0xc3, 0xc3,0xe7, 0x7e};
static GLubyte P_Rasters[13] = { 0x00,0x00, 0xc0,0xc0, 0xc0,0xc0, 0xc0,0xfe, 0xc7,0xc3, 0xc3,0xc7, 0xfe};
static GLubyte E_Rasters[13] = { 0x00,0x00, 0xff,0xc0, 0xc0,0xc0, 0xc0,0xfc, 0xc0,0xc0, 0xc0,0xc0, 0xff};
static GLubyte N_Rasters[13] = { 0x00,0x00, 0xc7,0xc7, 0xcf,0xcf, 0xdf,0xdb, 0xfb,0xf3, 0xf3,0xe3, 0xe3};
static GLubyte G_Rasters[13] = { 0x00,0x00, 0x7e,0xe7, 0xc3,0xc3, 0xcf,0xc0, 0xc0,0xc0, 0xc0,0xe7, 0x7e};
static GLubyte L_Rasters[13] = { 0x00,0x00, 0xff,0xc0, 0xc0,0xc0, 0xc0,0xc0, 0xc0,0xc0, 0xc0,0xc0, 0xc0};

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	DrawBitmap * example = new DrawBitmap(class_name);
	return reinterpret_cast<GLApplication *>(example);
}


/** 构造函数 */
DrawBitmap::DrawBitmap(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	//m_Angle = 0.0f;												/**< 设置初始角度为0 */
}

/** 初始化OpenGL */
bool DrawBitmap::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);	
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */
    
	glPixelStorei (GL_UNPACK_ALIGNMENT, 1);      /**<　指定像素数据存储模式 */
	return true;												/**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void DrawBitmap::Uninit()									
{
/** 用户自定义的卸载过程 */
///......
///......
}

/** 程序更新函数 */
void DrawBitmap::Update(DWORD milliseconds)						
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
     
}

/** 绘制函数 */
void DrawBitmap::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();											
	glTranslatef(0.0f, 0.0f, -6.0f);
	/** 绘制字符 OPENGL  */ 
	glColor3f (1.0, 0.0, 1.0); /**< 指定颜色 */
	glRasterPos2i (0, 0);   /**<　指定光栅位置 */
	///绘制字符
	glBitmap (8, 13, 0.0, 0.0, 15.0, 0.0, O_Rasters); /**< 绘制字符O */
	glBitmap (8, 13, 0.0, 0.0, 15.0, 0.0, P_Rasters); /**< 绘制字符P */
	glBitmap (8, 13, 0.0, 0.0, 15.0, 0.0, E_Rasters); /**< 绘制字符E */
	glBitmap (8, 13, 0.0, 0.0, 15.0, 0.0, N_Rasters); /**< 绘制字符N */
	glBitmap (8, 13, 0.0, 0.0, 15.0, 0.0, G_Rasters); /**< 绘制字符G */
	glBitmap (8, 13, 0.0, 0.0, 15.0, 0.0, L_Rasters); /**< 绘制字符L */
	
   glFlush();													/**< 强制执行所有的OpenGL命令 */
}
