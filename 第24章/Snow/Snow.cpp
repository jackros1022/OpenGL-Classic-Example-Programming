//=========================================================================
/**
*  @file      Snow.cpp
*
*  项目描述： 粒子系统
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-18
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
#include "Snow.h"						    


/** 用来设置粒子的属性值 */
float x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec;
int r,g,b;


/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Test * test = new Test(class_name);
	return reinterpret_cast<GLApplication *>(test);
}


/** 构造函数 */
Test::Test(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量
	Snow = new CParticle();
	m_Fps = 0;
	angle =0;
	
}

/** 初始化OpenGL */
bool Test::Init()									
{
/** 用户自定义的初始化过程 */
	glClearColor(0.0f, 0.0f, 0.0f, 0.5f);						
	glClearDepth(1.0f);											
	glDepthFunc(GL_LEQUAL);										
	glEnable(GL_DEPTH_TEST);
	glShadeModel(GL_SMOOTH);									
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);			
	ResizeDraw(true);	                             
	 
	glEnable(GL_TEXTURE_2D);             /**< 开启纹理映射 */      
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);    /**< 设置混合因子获得半透明效果 */
	glEnable(GL_BLEND);				     /**< 启用混和 */

	/** 初始化字体 */
	if(!m_Font.InitFont())
	{
		MessageBox(NULL,"初始化字体失败!","错误",MB_OK);
		exit(-1);
	}
 
	/**  载入纹理 */
	if(!Snow->LoadTextures())
	{
		MessageBox(NULL,"载入纹理失败!","错误",MB_OK);
		exit(-1);
	}
	
	/** 创建500个粒子 */
	Snow->Create(500);
	
	/** 初始化粒子 */
	InitSnow();
	
	return true;                                        /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Test::Uninit()									
{
	
}

/** 程序更新函数 */
void Test::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}
	    
}
/** 计算帧速 */
void Test::CaculateFrameRate()
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
void  Test::PrintText()
{
	char string[128];                               /**< 用于保存输出字符串 */
	glPushAttrib(GL_CURRENT_BIT);                   /**< 保存现有颜色属性信息 */
	glColor3f(0.0f,1.0f,0.0f);                      /**< 设置文字颜色 */
	
	/** 输出帧速 */
	CaculateFrameRate();                               /**< 计算帧速 */
    sprintf(string,"FPS:%3.0f",m_Fps);                 /**< 字符串赋值 */
	m_Font.PrintText(string, -5.0f,3.0f);              /**< 输出字符串 */

	glPopAttrib();
}

/** 绘制地面 */
void Test::DrawGround()
{
	glPushMatrix();
	glTranslatef(0.0f,0.0f,-6.0f);
	glRotatef(angle,0.0f,1.0f,0.0f);   

	/** 指定纹理 */
	glBindTexture(GL_TEXTURE_2D,Snow->GetTexture(0));
	glColor4ub(255,255,255,255);
	glNormal3f(0.0f,1.0f,0.0f);
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f,0.0f);glVertex3f(-2.5f,-1.0f,2.5f);
		glTexCoord2f(1.0f,0.0f);glVertex3f(-2.5f,-1.0f,-2.5f);
		glTexCoord2f(1.0f,1.0f);glVertex3f(2.5f,-1.0f,-2.5f);
		glTexCoord2f(0.0f,1.0f);glVertex3f(2.5f,-1.0f,2.5f);
	glEnd();
	glPopMatrix();
	
	/** 地面转动的角度 */
	angle += 0.02f;
}

/** 初始化雪花粒子 */
bool Test::InitSnow()
{
	for (int i=0; i < Snow->GetNumOfParticle(); ++i)
	{
		///初始化颜色
		r = 255;
		g = 255;
		b = 255;

		Snow->SetColor(i,r,g,b);

		///初始化坐标
		x = 0.1f * (rand() % 50) - 2.5f;
		y = 2 + 0.1f * (rand() % 2);
		if((int)x % 2 == 0)
			z = rand()%6;
		else 
			x = -rand()%3;

		Snow->SetPosition(i,x,y,z);

		///初始化速度
		vx = 0.00001 * (rand()%100);
		vy = 0.0000002 * (rand()%28000);
		vz = 0;

		Snow->SetVelocity(i,vx,vy,vz);

		///初始化加速度
		ax = 0;
		ay = 0.000005f;
		az = 0;
		
		Snow->SetAcceleration(i,ax,ay,az);

		///初始化生命周期
		lifetime = 100;
		Snow->SetLifeTime(i,lifetime);

		///消失速度
		dec = 0.005 * (rand()%50);
		Snow->SetDec(i,dec);

		///初始化大小
		Snow->SetSize(i,0.03f);

	}
	return true;
}

/** 绘制粒子 */
void Test::DrawParticle()
{
	/** 绑定纹理 */
	glBindTexture(GL_TEXTURE_2D,Snow->GetTexture(1));
	
	for(int i = 0; i<Snow->GetNumOfParticle(); ++i)
	{
		/** 获得粒子的所有属性 */
		Snow->GetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
		glLoadIdentity();
		glTranslatef(0.0f,0.0f,-6.0f);
		glColor4ub(r,g,b,255);
		glNormal3f(0.0f,0.0f,1.0f);   /**< 定义法线方向 */
		/** 画出粒子 */
		glBegin(GL_QUADS);
			glTexCoord2f(0.0f,0.0f);glVertex3f(x-size,y-size,z);
			glTexCoord2f(1.0f,0.0f);glVertex3f(x-size,y+size,z);
			glTexCoord2f(1.0f,1.0f);glVertex3f(x+size,y+size,z);
			glTexCoord2f(0.0f,1.0f);glVertex3f(x+size,y-size,z);
		glEnd();
		
		/** 更新粒子属性 */
		UpdateSnow();
		Snow->SetAll(i,r,g,b,x,y,z,vx,vy,vz,ax,ay,az,size,lifetime,dec);
		
	}
}

/** 更新粒子 */
void Test::UpdateSnow()
{
	/** 更新位置 */
	x += (vx * 5);
	y -= vy;

	/** 更新速度 */
	vy += ay;
	
	/** 更新生存时间 */
	lifetime -= dec;
	
	if(x > 3)
		x = -2;

	/** 如果粒子消失或生命结束 */
	if (y <= -1 || lifetime <= 0)
	{
		/** 初始化位置 */
		x = 0.1f * (rand()%50) - 2.5f;
		y = 2 + 0.1f * (rand()%2);
		if((int)x%2 == 0)
			z = rand()%6;
		else 
			z = -rand()%3;
		
		/** 初始化速度 */
		vx = (float)(0.00001 * (rand()%100));
		vy = (float)(0.0000002 * (rand()%28000));
		vz = 0;
		
		/** 初始化加速度 */
		ax = 0;
		ay = 0.000005f;
		az = 0;
		lifetime = 100;
		dec = 0.005*(rand()%50);
	}
}

/** 绘制函数 */
void Test::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);			
	glLoadIdentity();
	
	/** 绘制地面 */
	DrawGround();

	/** 绘制粒子 */
	DrawParticle();

	/** 输出信息 */
	PrintText();
		
	glFlush();	                     /**< 强制执行所有的OpenGL命令 */
}
