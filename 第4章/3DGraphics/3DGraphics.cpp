
//=========================================================================
/**
*  @file  3DGraphics.cpp
*
*  项目描述： 3D图形的绘制
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-10-13	
*  修改日期： 2006-11-13
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
#include "3DGraphics.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>											


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							

/** 定义常量GL_PI */
static const GLfloat GL_PI = 3.14f;

/** 顶点数组 */
static GLfloat vertices[] = { 1.0f,1.0f,-1.0f, -1.0f,1.0f,-1.0f,
                              -1.0f, 1.0f, 1.0f,1.0f, 1.0f, 1.0f,
						      1.0f,-1.0f, 1.0f,-1.0f,-1.0f, 1.0f,
							  -1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f};
/** 顶点颜色数组 */
static GLfloat colors[] = {1.f,0.f,0.f,0.f,1.f,0.f,
                          0.f,0.f,1.f,1.f,1.f,0.f,
						  1.f,0.f,1.f,0.f,1.f,1.f,
                          0.4f,0.6f,0.3f,0.f,0.8f,0.4f};
/** 顶点索引数组 */
static GLbyte indexs[] = {0,1,2,3,/**< 顶面索引 */
                          4,5,6,7,/**< 底面索引 */
						  0,3,4,7,/**< 右面索引 */
						  2,5,4,3,/**< 前面索引 */
						  2,1,6,5,/**< 左面索引 */
						  0,1,6,7};/**< 后面索引 */
	                      

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Graphics * graphics = new Graphics(class_name);
	return reinterpret_cast<GLApplication *>(graphics);
}


/** 构造函数 */
Graphics::Graphics(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	m_Angle = 0.0f;												/**< 设置初始角度为0 */
	m_Pivot = 1;                                                /**< 设置初始值为0 */
		
}

/** 初始化OpenGL */
bool Graphics::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);
	
	/** 启用顶点数组和颜色数组 */
	glEnable(GL_VERTEX_ARRAY);
	glEnable(GL_COLOR_ARRAY);
	/** 指定数组中的数据 */
	glVertexPointer(3,GL_FLOAT,0,vertices);
	glColorPointer(3,GL_FLOAT,0,colors);
	
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */

	return true;												/**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Graphics::Uninit()									
{
/** 用户自定义的卸载过程 */
///......
///......
}

/** 程序更新函数 */
void Graphics::Update(DWORD milliseconds)						
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
void Graphics::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	/**< 清除颜色缓存和深度缓存 */		
	glLoadIdentity();
    
	/** 绘制圆锥体 */
	glPushMatrix();
	   glTranslatef(-1.5f,0.5f,-7.0f);	/**< 向左上角移，并移入屏幕 */						   glRotatef(m_Angle,1.0f,1.0f,0.0f);  
	   glShadeModel(GL_FLAT); /**< 设置为恒定着色 */
	   /** 绘制圆锥侧面 */
	   glBegin(GL_TRIANGLE_FAN);
	       glVertex3f(0.f,1.8f,0.f);/**< 指定圆锥顶点 */
		   for(float angle = 0.0f; angle < 2.0f *GL_PI ; angle += GL_PI/8.0f)
		   {
			  if(m_Pivot % 2 == 0)
				  glColor3f(0.f,1.f,0.f);
			  else
				  glColor3f(0.f,0.0f,1.0f);
			 
			  m_Pivot++;
			  glVertex3f(0.8*sin(angle),0.f,0.8*cos(angle));/**< 指定圆锥底面上点 */
		   }
		glEnd();
       
		/** 下面绘制圆锥底面 */
	   glBegin(GL_TRIANGLE_FAN);
	     glVertex2f(0.0f,0.f);                             /**< 指定圆心坐标 */
		 for(float angle = 0.0f; angle < 2.0 * GL_PI; angle +=GL_PI/8.0f)
          {
			  if(m_Pivot % 2 == 0)
				  glColor3f(0.f,1.f,0.f);
			  else
				  glColor3f(0.f,0.0f,1.0f);
			  
			  m_Pivot++;
			  glVertex3f(0.8* sin(angle),0.f,0.8 * cos(angle));/**< 指定圆周上的点 */
		   }
	 glEnd();                                                  /**< 绘制结束 */
   glPopMatrix(); 
	  
   /** 绘制立方体 */
   glPushMatrix(); 
	glTranslatef(1.5f,1.0f,-7.0f);						/**< 向右上角移，并移入屏幕 */
	glRotatef(m_Angle,1.0f,1.0f,1.0f);					/**< 在XYZ轴上旋转立方体 */
	glScalef(0.75f,0.75f,0.75f);
	glShadeModel(GL_SMOOTH);                            /**< 使用平滑着色 */
    ///开始绘制	
	glBegin(GL_QUADS);
	  for(int i = 0; i < 6; i++)                         /**< 使用顶点数组来绘制立方体的六个面 */
		{ 
		    glArrayElement(indexs[4 * i]);
			glArrayElement(indexs[4 * i + 1]);
			glArrayElement(indexs[4 * i + 2]);
			glArrayElement(indexs[4 * i + 3]);
		}
	glEnd();                                /**< 绘制结束 */
   glPopMatrix();

  /**　绘制线框球 */
   glPushMatrix();
	 glTranslatef(-1.5f,-1.5f,-7.0f);
	 glRotatef(m_Angle,0.0f,1.0f,1.0f);
	 glColor3f(0.3f,0.7f,0.6f);
	 auxWireSphere(1.0);                          /**< 使用aux辅助库函数生成一个线框球 */
   glPopMatrix();

  /** 绘制线框茶壶 */
    glPushMatrix();
	  glTranslatef(1.5f,-1.5f,-7.0f);
	  glRotatef(m_Angle,0.0f,1.0f,0.0f);
	  glColor3f(0.3f,0.6f,0.2f);
	  auxWireTeapot(1.0);                        /**< 使用aux辅助库函数生成一个线框茶壶 */
    glPopMatrix();

	glFlush();									/**< 强制执行所有的OpenGL命令 */
}
