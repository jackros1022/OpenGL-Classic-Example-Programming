//=========================================================================
/**
*  @file  Fog.cpp
*
*  项目描述： OpenGL雾化效果
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-01
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

#include "Fog.h"											/**< 包含头文件 */

#include <gl\gl.h>												/**< 包含OpenGL头文件 */
#include <gl\glu.h>												
#include <gl\glaux.h>	 
                         
#pragma comment(lib, "opengl32.lib")							/**< 包含OpenGL链接库文件 */
#pragma comment(lib, "glu32.lib")							
#pragma comment(lib, "glaux.lib")							

/** 定义光源的属性值 */
GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f }; 	/**< 环境光参数 */
GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };		/**< 漫射光参数 */
GLfloat LightSpecular[]= { 1.0f, 1.0f, 1.0f, 1.0f };	/**< 镜面光参数 */
GLfloat LightPosition[]= { 0.0f, 0.0f, 0.0f, 1.0f };	/**< 光源位置 */

/** 定义材质属性 */
GLfloat mat_ambient[]={0.0,0.1,0.8,1.0};  /**< 环境颜色 */
GLfloat mat_diffuse[]={0.0,0.3,0.6,1.0};  /**< 散射颜色 */
GLfloat mat_specular[]={1.0,0.0,1.0,1.0}; /**< 镜面反射颜色 */
GLfloat mat_shininess[]={10.0};           /**< 镜面反射指数 */

GLuint fogMode[]= { GL_LINEAR, GL_EXP, GL_EXP2 };		/**< 雾效的模式 */
GLuint fogFilter= 0;									/**< 使用哪一种雾效模式 */
GLfloat fogColor[4]={0.5,0.5,0.5,1.0};                  /**< 雾的颜色 */ 

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Fog * fog = new Fog(class_name);
	return reinterpret_cast<GLApplication *>(fog);
}


/** 构造函数 */
Fog::Fog(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	z = -5.0f;             /**< 初始化为-5.0f */
	fp = false;            /**< 初始化为flase */
}



/** 初始化OpenGL */
bool Fog::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.5f,0.5f,0.5f,1.0f);
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);									
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	
    /** 设置光源的属性值 */
	glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);				/**< 设置环境光 */
	glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);				/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_SPECULAR, LightSpecular);			/**< 设置漫射光 */
	glLightfv(GL_LIGHT1, GL_POSITION,LightPosition);			/**< 设置光源位置 */
    /** 设置材质的属性值 */
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);              /**< 设置环境颜色 */
    glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);              /**< 设置散射颜色 */
    glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);            /**< 设置镜面反射颜色 */
    glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);          /**< 设置镜面反射指数 */

	/** 启用光照和光源 */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT1);     
    /** 启用雾效 */
	glEnable(GL_FOG); 
    { 
		glFogi(GL_FOG_MODE,fogMode[fogFilter]); /** 设置雾效的模式 */
		glFogfv(GL_FOG_COLOR,fogColor);         /** 指定雾的颜色 */ 
       	glFogf(GL_FOG_DENSITY,0.2f);            /** 设置雾的浓度 */
		glFogf(GL_FOG_START,1.0);               /** 设置线性雾的开始位置 */
		glFogf(GL_FOG_END,10.0);                /** 设置线性雾的结束位置 */
		glHint(GL_FOG_HINT,GL_DONT_CARE);       /** 规定雾化效果的质量 */  
    } 
	ResizeDraw(true);						   /**< 改变OpenGL窗口大小，直接调用子类的函数 */
	
  	return true;                               /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Fog::Uninit()									
{
}

/** 程序更新函数 */
void Fog::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}
  
 /** 用户自定义的更新过程 */

	if(m_Keys.IsPressed(VK_DOWN))             /**< 向下方向键被按下了么？*/
		z -= 0.02f;                           /**< 将模型移入屏幕内部 */

	if(m_Keys.IsPressed(VK_UP))               /**< 向上方向键被按下了么？*/
		z += 0.02f;                           /**< 将所有的球移向观察者 */

	if(m_Keys.IsPressed('F') && !fp )         /**< F键是否被按下？*/
	{
		fp = true;                            /**< 是 */
		fogFilter += 1;                       /**< 变换雾效模式 */
		if(fogFilter > 2)                     /**< fogFilter是否大于2 */
		{
			fogFilter = 0;                    /**< 置零 */
    	}
		glFogi (GL_FOG_MODE, fogMode[fogFilter]); /**< 设置雾效模式 */
	}
	if(!m_Keys.IsPressed('F'))                /**< F键是否被释放？*/
		fp = false;                           /**< 是，则设置为释放 */

	    
}
/** 绘制球体 */
void Fog::DrawSphere(float x,float y,float z)
{
	glPushMatrix();
	glTranslatef(x,y,z);     /**< 移到指定位置 */
	auxSolidSphere(0.5);     /**< 绘制一个球体 */
	glPopMatrix();
}

/** 绘制函数// */
void Fog::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	 /**< 清楚缓存 */		
	glLoadIdentity();
	/** 绘制过程 */	
	for(int i=1; i < 10; i++)         /**< 循环10次绘制10个球体 */
	    DrawSphere(-3 + i * 0.8, 0.0, z - i + 1);	
   
    glFlush();				           /**< 强制执行所有的OpenGL命令 */
}
