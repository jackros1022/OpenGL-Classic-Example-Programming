//=========================================================================
/**
*  @file      Tess.cpp
*
*  项目描述： OpenGL网格化
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-02
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

#include "Tess.h"						    /**< 包含头文件 */

#include <gl\gl.h>							/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	
#include <stdio.h>

#pragma comment(lib, "opengl32.lib")		/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")	

#ifndef CALLBACK                            /**< 对符号CALLBACK使用空定义 */
#define CALLBACK
#endif

/** 多边形的绘制模式 */
GLuint polygonMode[] = { GL_FILL, GL_LINE }; /**< 填充模式和线框模式 */
GLuint polyMode = 0;                         /**< 多边形绘制模式索引 */

/** 环绕规则 */
GLdouble TessProperty[5] = 
{
	GLU_TESS_WINDING_ODD,        /**< 环绕数为奇数 */
	GLU_TESS_WINDING_NONZERO,    /**< 环绕数为非0 */
	GLU_TESS_WINDING_POSITIVE,   /**< 环绕数为正数 */
	GLU_TESS_WINDING_NEGATIVE,   /**< 环绕数为负数 */
	GLU_TESS_WINDING_ABS_GEQ_TWO /**< 环绕数绝对值大于等于2 */
 };
GLuint nProperty=0;              /**< 环绕规则索引 */

/** 定义字符串,用于输出信息 */
char * TessWinding[] = 
{
	"GLU_TESS_WINDING_ODD",        /**< 环绕数为奇数 */
	"GLU_TESS_WINDING_NONZERO",    /**< 环绕数为非0 */
	"GLU_TESS_WINDING_POSITIVE",   /**< 环绕数为正数 */
	"GLU_TESS_WINDING_NEGATIVE",   /**< 环绕数为负数 */
	"GLU_TESS_WINDING_ABS_GEQ_TWO" /**< 环绕数绝对值大于等于2 */
};
/** gluTessCallback注册的回调函数 */
void CALLBACK beginCallback(GLenum which)
{
   glBegin(which);
}

void CALLBACK errorCallback(GLenum errorCode)
{
   const GLubyte *string;
   ///输出错误信息
   string = gluErrorString(errorCode);
   fprintf(stderr, "Tessellation Error: %s\n", string);
   exit(0);
}

void CALLBACK endCallback(void)
{
   glEnd();
}

void CALLBACK vertexCallback(GLvoid *vertex)
{
   
 GLdouble* pt;
 GLubyte red, green, blue;
 pt = (GLdouble* )vertex;
 /** 随即产生颜色值 */
 red   = (GLubyte)rand() & 0xff;   
 green = (GLubyte)rand() & 0xff;  
 blue  = (GLubyte)rand() & 0xff; 
 glColor3ub(red, green, blue);
 glVertex3dv(pt);

}

/** 用于处理检测轮廓线交点，并决定是否合并顶点，
    新创建的顶点最多可以是4个已有顶点线性组合，这些定点坐标存储在data中
    其中weight为权重，weight[i]的总合为1.0
 */
void CALLBACK combineCallback(GLdouble coords[3], 
                              GLdouble *vertex_data[4],
                              GLfloat weight[4], 
							  GLdouble **dataOut )
{
   GLdouble *vertex;
   int i;
   /** 分配存储新顶点的内存 */
   vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
   
   /** 存储坐标值 */
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
   
   /** 通过插值计算RGB颜色值 */
   for (i = 3; i < 6; i++)
      vertex[i] = weight[0] * vertex_data[0][i] + 
                  weight[1] * vertex_data[1][i] + 
                  weight[2] * vertex_data[2][i] + 
                  weight[3] * vertex_data[3][i];
   *dataOut = vertex;
}

/** 初始化网格对象 */
bool Tess::InitTess()
{
	/** 定义外面矩形坐标 */
	GLdouble rectOut[4][3] = { -5.0, -5.0, 0.0,
                                5.0, -5.0, 0.0,
                                5.0,  5.0, 0.0,
                               -5.0,  5.0, 0.0};
	/** 定义内矩形坐标 */
    GLdouble rectIn[4][3] = { -2.0, -2.0, 0.0,
		                       2.0, -2.0, 0.0, 
							   2.0,  2.0, 0.0,
		                      -2.0,  2.0, 0.0						   
                              };
   /** 定义两个三角形坐标 */
   GLdouble star[6][3] = { -5.1, -3.0, 0.0, 5.1, -3.0, 0.0,  0.0, 6.0, 0.0, 
                            0.0, -6.0, 0.0, 5.1,  3.0, 0.0, -5.1, 3.0, 0.0};

   
   list = glGenLists(2);/**< 获取两个显示列表索引 */
   
   tobj = gluNewTess(); /**< 创建网格化对象 */
   /** 注册回调函数 */
   gluTessCallback(tobj, GLU_TESS_VERTEX, 
                   (void(__stdcall*)())glVertex3dv);
   gluTessCallback(tobj, GLU_TESS_BEGIN, 
                   (void(__stdcall*)())beginCallback);
   gluTessCallback(tobj, GLU_TESS_END, 
                   (void(__stdcall*)())endCallback);
   gluTessCallback(tobj, GLU_TESS_ERROR, 
                   (void(__stdcall*)())errorCallback);
   /** 创建一个显示列表*/
   glNewList(list, GL_COMPILE);
   /** 设置环绕规则 */
   gluTessProperty(tobj, GLU_TESS_WINDING_RULE,
                   TessProperty[nProperty]);
   /** 开始描述一个多边形 */    
   gluTessBeginPolygon(tobj, NULL);
      gluTessBeginContour(tobj);
         gluTessVertex(tobj, rectOut[0], rectOut[0]);
         gluTessVertex(tobj, rectOut[1], rectOut[1]);
         gluTessVertex(tobj, rectOut[2], rectOut[2]);
         gluTessVertex(tobj, rectOut[3], rectOut[3]);
      gluTessEndContour(tobj);
      
	  gluTessBeginContour(tobj);
         gluTessVertex(tobj, rectIn[0], rectIn[0]);
         gluTessVertex(tobj, rectIn[1], rectIn[1]);
         gluTessVertex(tobj, rectIn[2], rectIn[2]);
		  gluTessVertex(tobj, rectIn[3], rectIn[3]);
      gluTessEndContour(tobj);
   gluTessEndPolygon(tobj);
   glEndList();           /**< 结束显示列表 */

   /** 注册回调函数 */
   gluTessCallback(tobj, GLU_TESS_VERTEX, 
                   (void(__stdcall*)())vertexCallback);
   gluTessCallback(tobj, GLU_TESS_BEGIN, 
                   (void(__stdcall*)())beginCallback);
   gluTessCallback(tobj, GLU_TESS_END, 
                   (void(__stdcall*)())endCallback);
   gluTessCallback(tobj, GLU_TESS_ERROR, 
                   (void(__stdcall*)())errorCallback);
   gluTessCallback(tobj, GLU_TESS_COMBINE, 
                   (void(__stdcall*)())combineCallback);
   
   /** 创建第二个显示列表 */
   glNewList(list + 1, GL_COMPILE);
   /** 设置环绕规则 */
   gluTessProperty(tobj, GLU_TESS_WINDING_RULE,
                   TessProperty[nProperty]);
   /** 开始描述一个多边形 */
   gluTessBeginPolygon(tobj, NULL);
      gluTessBeginContour(tobj);     /**< 定义轮廓线1 逆时针三角形 */
		gluTessVertex(tobj, star[0], star[0]);
		gluTessVertex(tobj, star[1], star[1]);
		gluTessVertex(tobj, star[2], star[2]);
      gluTessEndContour(tobj);
	  
	  gluTessBeginContour(tobj);     /**< 定义轮廓线1 逆时针三角形 */
		gluTessVertex(tobj, star[3], star[3]);
		gluTessVertex(tobj, star[4], star[4]);
		gluTessVertex(tobj, star[5], star[5]);
      gluTessEndContour(tobj);
  gluTessEndPolygon(tobj);
  glEndList();                /**< 结束显示列表 */

  return true;
}
/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Tess * tess = new Tess(class_name);
	return reinterpret_cast<GLApplication *>(tess);
}


/** 构造函数 */
Tess::Tess(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	m_Angle = 0.0f;				/**< 设置初始角度为0 */
	wp = false;
	sp = false;
	
}

/** 初始化OpenGL */
bool Tess::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);	     /**< 改变OpenGL窗口大小，直接调用子类的函数 */
    /** 初始化网格对象 */
	if(!InitTess())
	{
		MessageBox(NULL,"初始化网格化对象失败!","错误",MB_OK);
		return false;
	}
	if(!font.InitFont())
	{
		MessageBox(NULL,"初始化字体失败！","错误",MB_OK);
	}

	return true;             /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Tess::Uninit()									
{
/** 用户自定义的卸载过程 */
   if(list != 0)             /**< 删除显示列表索引 */
	   glDeleteLists(list,2);  
   gluDeleteTess(tobj);      /**< 删除网格化对象 */

 }

/** 程序更新函数 */
void Tess::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

/** 用户自定义的更新过程 */
	if(m_Keys.IsPressed('W') && !wp )          /**< W键是否被按下？*/
	{
		wp = true;                             /**< 是 */
        
		polyMode += 1;
		if(polyMode > 1)                       
			polyMode = 0;                      /**< 置零 */
		/** 设置多边形绘制模式 */
		glPolygonMode(GL_FRONT_AND_BACK,polygonMode[polyMode]);

	}
	if(!m_Keys.IsPressed('W'))                /**< W键是否被释放？*/
	{
		wp = false;                           /**< 是，则设置为释放 */
	}

	if(m_Keys.IsPressed(VK_SPACE) && !sp )     /**< 空格键是否被按下？*/
	{
		sp = true;                             /**< 是 */
        
		nProperty += 1;
		if(nProperty > 4)
			nProperty = 0;                     /**< 置零 */
		InitTess();                            /**< 初始化网格对象 */

	}
	if(!m_Keys.IsPressed(VK_SPACE))            /**< 空格键是否被释放？*/
	{
		sp = false;                            /**< 是，则设置为释放 */
		
	}
    
	/** 更新旋转的角度 */
	m_Angle += (float)(milliseconds) / 15.0f;
     
}

/** 绘制函数 */
void Tess::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();	
	//glTranslatef(0.0f, 0.0f, -30.0f);
	/** 绘制带空的正方形 */	
	glPushMatrix();
	  glTranslatef(-6.0f, 0.0f, -30.0f);
	  glRotatef(m_Angle,1.0f,1.0f,1.0f);
	  glColor3f(1.0f,1.0f,0.0f);
	  glCallList(list);
	glPopMatrix();
    
	/** 绘制互倒的三角形组成一个六角星 */
	glPushMatrix();
	  glTranslatef(6.0f, 0.0f, -30.0f);
	  glRotatef(m_Angle,1.0f,1.0f,1.0f);
	  glCallList(list + 1);
	glPopMatrix();
    /** 输出一些信息 */
	glPushMatrix();
	glTranslatef(0.0f,0.0f,-1.0f);
	glColor3f(0.0f,1.0f,1.0f);              /**< 指定颜色 */
	char string[128];                       /**< 输出当前环绕模式 */
	sprintf(string, "当前环绕模式：%s",TessWinding[nProperty]);
	font.PrintText(string,-5.0f,3.0f);
	sprintf(string,"空格键---切换环绕模式");
	font.PrintText(string,-5.0f,2.0f);
    glPopMatrix();

	glFlush();							    /**< 强制执行所有的OpenGL命令 */
}
