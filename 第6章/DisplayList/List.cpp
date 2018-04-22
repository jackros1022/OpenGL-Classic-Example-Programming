//=========================================================================
/**
*  @file  List.cpp
*
*  项目描述： OPenGL显示列表
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-11-19
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

#include "List.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>											


#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							


/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	DisplayList * list = new DisplayList(class_name);
	return reinterpret_cast<GLApplication *>(list);
}


/** 构造函数 */
DisplayList::DisplayList(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	m_Angle = 0.0f;												/**< 设置初始角度为0 */
	m_Pyramid = 0;
	
}

/** 初始化OpenGL */
bool DisplayList::Init()									
{
/** 用户自定义的初始化过程 */
	
	m_Pyramid = glGenLists(1); /**< 分配1个未被使用的索引号 */
	if(m_Pyramid != 0)
	{
		///创建一个显示列表
		glNewList(m_Pyramid,GL_COMPILE); 
		 ///绘制一个金字塔体
		  glBegin(GL_TRIANGLES);
			///前侧面
			glColor3f(1.0f,0.0f,0.0f);			/**< 红色 */
			glVertex3f( 0.0f, 1.0f, 0.0f);		/**< 三角形的上顶点 (前侧面) */
			glColor3f(0.0f,1.0f,0.0f);			/**< 绿色 */
			glVertex3f(-1.0f,-1.0f, 1.0f);		/**< 三角形的左下顶点 (前侧面) */
			glColor3f(0.0f,0.0f,1.0f);			/**< 蓝色 */
			glVertex3f( 1.0f,-1.0f, 1.0f);		/**< 三角形的右下顶点 (前侧面) */
            //右侧面
			glColor3f(1.0f,0.0f,0.0f);			/**< 红色 */
			glVertex3f( 0.0f, 1.0f, 0.0f);		/**< 三角形的上顶点 (右侧面) */
			glColor3f(0.0f,0.0f,1.0f);			/**< 蓝色 */
			glVertex3f( 1.0f,-1.0f, 1.0f);		/**< 三角形的左下顶点 (右侧面) */
			glColor3f(0.0f,1.0f,0.0f);			/**< 绿色 */
			glVertex3f( 1.0f,-1.0f, -1.0f);		/**< 三角形的右下顶点 (右侧面) */
			///后侧面
			glColor3f(1.0f,0.0f,0.0f);			/**< 红色 */
			glVertex3f( 0.0f, 1.0f, 0.0f);		/**< 三角形的上顶点 (后侧面) */
			glColor3f(0.0f,1.0f,0.0f);			/**< 绿色 */
			glVertex3f( 1.0f,-1.0f, -1.0f);		/**< 三角形的左下顶点 (后侧面) */
			glColor3f(0.0f,0.0f,1.0f);			/**< 蓝色 */
			glVertex3f(-1.0f,-1.0f, -1.0f);		/**< 三角形的右下顶点 (后侧面) */
            ///作侧面
			glColor3f(1.0f,0.0f,0.0f);			/**< 红色 */
			glVertex3f( 0.0f, 1.0f, 0.0f);		/**< 三角形的上顶点 (左侧面) */
			glColor3f(0.0f,0.0f,1.0f);			/**< 蓝色 */
			glVertex3f(-1.0f,-1.0f,-1.0f);		/**< 三角形的左下顶点 (左侧面) */
			glColor3f(0.0f,1.0f,0.0f);			/**< 绿色 */
			glVertex3f(-1.0f,-1.0f, 1.0f);		/**< 三角形的右下顶点 (左侧面) */
		  glEnd();
		   
		  ///绘制底面四边形	
		  glBegin(GL_QUADS);
			glColor3f(1.0f,0.0f,0.0f);
			glVertex3f(-1.0f,-1.0f, 1.0f);
			glColor3f(0.0f,1.0f,0.0f);
			glVertex3f( 1.0f,-1.0f, 1.0f);
			glColor3f(0.0f,0.0f,1.0f);
			glVertex3f( 1.0f,-1.0f, -1.0f);
			glColor3f(1.0f,0.0f,1.0f);
			glVertex3f(-1.0f,-1.0f, -1.0f);
		  glEnd();
         //glTranslatef(3.0f,0.0f,0.0f); /**< 增加偏移,为下一次绘制作准备 */
	   glEndList();/**< 结束显示列表 */
	}
	///开始初始化	
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);	
	glDepthFunc(GL_LEQUAL);	
	
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);     /**< 采用平滑着色 */
		
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//glEnable(GL_COLOR_MATERIAL);
	ResizeDraw(true);											/**< 改变OpenGL窗口大小，直接调用子类的函数 */
	return true;												/**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void DisplayList::Uninit()									
{
/** 用户自定义的卸载过程 */
///......
///......
}

/** 程序更新函数 */
void DisplayList::Update(DWORD milliseconds)						
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

	m_Angle += (float)milliseconds / 30;  /**< 更新物体旋转角度 */

}

/** 绘制函数 */
void DisplayList::Draw()											
{
     /** 用户自定义的绘制过程 */
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		/**< 清除颜色和深度缓存 */	
	glLoadIdentity();	
	
	///循环绘制12个金字塔体
	for(int y =0; y < 3; y++)  /**< 沿Y轴循环 */
	{
		for(int x=0; x < 4; x++)  /**< 沿X轴循环 */
		{
			glLoadIdentity();  /**< 重置模型变化矩阵 */
			glTranslatef(-3.0 + 2.0 * x , -2.0 + 2.0 * y, -10.0f);
			glScalef(0.8f,0.8f,0.8f);
			glRotatef(m_Angle,1.0f,1.0f,0.0f);
			glCallList(m_Pyramid); /**< 绘制金字塔体 */
			
		}
	}

   glFlush();													/**< 强制执行所有的OpenGL命令 */
}
