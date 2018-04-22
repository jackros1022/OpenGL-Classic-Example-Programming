//=========================================================================
/**
*  @file      SkyAndTerrain.cpp
*
*  项目描述： TGA文件载入
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
#include "stdafx.h"
#include "TGAFile.h"						    


/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	TGAFile * test = new TGAFile(class_name);
	return reinterpret_cast<GLApplication *>(test);
}


/** 构造函数 */
TGAFile::TGAFile(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	m_Fps = 0;
	m_RenderMode = true;
	sp = false;
}

/** 初始化OpenGL */
bool TGAFile::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);	                             /**< 改变OpenGL窗口大小，直接调用子类的函数 */
	
	/** 启用纹理 */
	glEnable(GL_TEXTURE_2D);

	/** 初始化字体 */
	if(!m_Font.InitFont())
		MessageBox(NULL,"初始化字体失败!","错误",MB_OK);
	
	/** 初始化地形 */	
	if(!m_Terrain.initTerrain())
	{
		MessageBox(NULL,"初始化地形失败!","错误",MB_OK);
		exit(0);
	}

	/** 初始化天空 */
	if(!m_SkyBox.Init())
	{
		MessageBox(NULL,"初始化天空失败!","错误",MB_OK);
		exit(0);
	}

	/** 载入TGA纹理 */
	if(!m_TGATexture[0].Load("data/sphere.tga"))
	{
		MessageBox(NULL,"加载TGA纹理失败!","错误",MB_OK);
		exit(0);
	}

	if(!m_TGATexture[1].Load("data/box.tga"))
	{
		MessageBox(NULL,"加载TGA纹理失败!","错误",MB_OK);
		exit(0);
	}
	
	/** 设置摄像机 */
	m_Camera.setCamera( 270, 35, 656,  268, 35, 654,  0, 1, 0);

	return true;                                        /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void TGAFile::Uninit()									
{
}
/** 更新摄像机 */
void TGAFile::UpdateCamera()
{
	m_Camera.setViewByMouse();
	
	/** 键盘按键响应 */
	if(m_Keys.IsPressed(VK_SHIFT))                        /**< 按下SHIFT键时加速 */
	{
		m_Camera.setSpeed(0.5f);
	}
	if(!m_Keys.IsPressed(VK_SHIFT))
	{
		m_Camera.setSpeed(0.2f);
	}
	if(m_Keys.IsPressed(VK_UP) || m_Keys.IsPressed('W'))   /**< 向上方向键或'W'键按下 */
		m_Camera.moveCamera(m_Camera.getSpeed());          /**< 移动摄像机 */

	if(m_Keys.IsPressed(VK_DOWN) || m_Keys.IsPressed('S')) /**< 向下方向键或'S'键按下 */
		m_Camera.moveCamera(-m_Camera.getSpeed());         /**< 移动摄像机 */

	if(m_Keys.IsPressed(VK_LEFT) || m_Keys.IsPressed('A')) /**< 向左方向键或'A'键按下 */
		m_Camera.yawCamera(-m_Camera.getSpeed());          /**< 移动摄像机 */

	if(m_Keys.IsPressed(VK_RIGHT) || m_Keys.IsPressed('D')) /**< 向右方向键或'D'键按下 */
		m_Camera.yawCamera(m_Camera.getSpeed());            /**< 移动摄像机 */

	/** 根据地形高度更新摄像机 */
	Vector3 vPos = m_Camera.getPosition();                  /**< 得到当前摄像机位置 */
    Vector3 vNewPos = vPos; 
	
	/** 设置摄像机高度为 地形高度 + 10 */
	vNewPos.y = (float)m_Terrain.getAveHeight(vPos.x,vPos.z ) + 10;

		/** 得到高度差值 */
		float temp = vNewPos.y - vPos.y;

		/** 更新摄像机方向 */
		Vector3 vView = m_Camera.getView();
		vView.y += temp;

		/** 设置摄像机 */
		m_Camera.setCamera(vNewPos.x,  vNewPos.y,  vNewPos.z,
						   vView.x,	   vView.y,	   vView.z,	 
						   0, 1, 0);								
	
}
/** 程序更新函数 */
void TGAFile::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

/** 用户自定义的更新过程 */
	
	/** 更新摄像机 */
	UpdateCamera();

	/** 空格键切换绘制模式 */
	if(m_Keys.IsPressed(VK_SPACE) && !sp)
	{
		sp = true;
		m_RenderMode = !m_RenderMode;
		if(m_RenderMode)
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		else
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if(!m_Keys.IsPressed(VK_SPACE))
		sp = false;
	
	    
}

/** 计算帧速 */
void TGAFile::CaculateFrameRate()
{
	static float framesPerSecond    = 0.0f;	     /**< 保存显示帧数 */	
    static float lastTime			= 0.0f;	     /**< 记录上次时间 */						
    float currentTime = GetTickCount() * 0.001f; /**< 获得当前时间 */	 			

	++framesPerSecond;                           /**< 显示帧数递增1 */
    /** 如果时间差大于1.0秒 */
	if( currentTime - lastTime > 1.0f )          
    {
		
	    lastTime = currentTime;                   /**< 保存当前时间 */
		m_Fps = framesPerSecond;                  /**< 当前帧数传给m_Fps */
        framesPerSecond = 0;                      /**< 将帧数置零 */                    
    }
}

/** 输出文字信息 */
void  TGAFile::PrintText()
{
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */
	glColor3f(0.0f,1.0f,0.0f);                      /**< 设置文字颜色 */
	sprintf(string,"当前位置:X=%3.1f  Y=%3.1f Speed =%3.1f ",   
		m_Camera.getPosition().x,m_Camera.getPosition().z ,m_Camera.getSpeed()); /**< 字符串赋值 */
	m_Font.PrintText(string,-5.0f,3.5f);

	/** 输出帧速 */
	CaculateFrameRate();                               /**< 计算帧速 */
    sprintf(string,"FPS:%3.0f",m_Fps);                 /**< 字符串赋值 */
	m_Font.PrintText(string, -5.0f,3.0f);              /**< 输出字符串 */
	glPopAttrib();
		
}

/** 绘制球体 */
void TGAFile::DrawSphere(float x,float z,float radius)
{
	/** 指定纹理 */
	glBindTexture(GL_TEXTURE_2D,m_TGATexture[0].ID);
	
	float y = m_Terrain.getAveHeight(x,z);
	
	/** 绘制过程 */
	glPushMatrix();
	glTranslatef(x,y + radius,z);
	  GLUquadricObj * sphere = gluNewQuadric();
      gluQuadricOrientation(sphere, GLU_OUTSIDE);
	  gluQuadricNormals(sphere,GLU_SMOOTH);
	  gluQuadricTexture(sphere,GL_TRUE);
	  gluSphere(sphere,radius,50,50);
	  gluDeleteQuadric(sphere);
	glPopMatrix();

}

/** 绘制木箱 */
void TGAFile::DrawBox(float x,float z,float length)
{
	/** 指定纹理 */
	glBindTexture(GL_TEXTURE_2D,m_TGATexture[1].ID);

	float y = m_Terrain.getAveHeight(x,z);

	glPushMatrix();
	glTranslatef(x,y + length ,z);
	glScalef(length,length,length);
	glPushMatrix();
	
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
	glEnd();
	glPopMatrix();
}

/** 绘制函数 */
void TGAFile::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	/** 放置摄像机 */	
	m_Camera.setLook();
	
	/** 渲染地形 */
	m_Terrain.render();
	
	/** 绘制天空 */
	m_SkyBox.CreateSkyBox(0, 0, 0, 1.0, 0.5,1.0);

	/** 绘制小球 */
	DrawSphere(230,515,10);

	/** 绘制木箱 */
	DrawBox(135,555,10);
	/** 输出屏幕信息 */ 
	PrintText();
		
	glFlush();	                 /**< 强制执行所有的OpenGL命令 */
}
