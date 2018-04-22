//=========================================================================
/**
*  @file      Test.cpp
*
*  项目描述： emboss凹凸映射
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-16
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
#include "Emboss.h"	

/** 多重纹理函数 */
PFNGLACTIVETEXTUREARBPROC       glActiveTextureARB       = NULL;
PFNGLMULTITEXCOORD2FARBPROC     glMultiTexCoord2fARB     = NULL;
PFNGLCLIENTACTIVETEXTUREARBPROC glClientActiveTextureARB = NULL;

/** 定义几个标志变量 */
bool  g_bEmbossBumpMap  = true;        /**< 是否启用凹凸映射 */
float g_fEmbossFactor   = 0.5f;        /**< 纹理坐标偏移因子 */

/** 物体顶点 */
Vertex g_quadVertices[NUM_VERTICES] = 
{
///    tu    tv     nx    ny     nz     x      y     z 
    { 0.0f, 0.0f,  0.0f, 0.0f, 1.0f,  -1.0f, -1.0f, 0.0f },
    { 1.0f, 0.0f,  0.0f, 0.0f, 1.0f,   1.0f, -1.0f, 0.0f },
    { 0.0f, 1.0f,  0.0f, 0.0f, 1.0f,  -1.0f,  1.0f, 0.0f },
    { 1.0f, 1.0f,  0.0f, 0.0f, 1.0f,   1.0f,  1.0f, 0.0f }
};

TexCoords g_fEmbossTexCoords[NUM_VERTICES];    /**< 保存偏移后的纹理坐标 */
Vector3 g_vTangents[NUM_VERTICES];             /**< 保存顶点的切线T分量 */
Vector3 g_vBiNormals[NUM_VERTICES];            /**< 保存顶点的切线B分量 */


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
	sp = false;
		
}

/** 检查是否支持扩展 */
bool Test::isExtensionSupported(const char* string)
{
	char *extension;			/**< 指向扩展字符串的指针 */
	char *end;				    /**< 最后一个字符指针 */
	int idx;

	extension = (char*)glGetString(GL_EXTENSIONS);
	if (extension == NULL)
		return false;	
	
	/** 得到最后一个字符 */
	end = extension + strlen(extension);	

	/** 循环查找字符串string */
	while (extension < end)
	{
		/** 找到空格前的一个字符串 */
		idx = strcspn(extension, " ");
		
		/** 检查是否找到字符串string */
		if ( (strlen(string) == idx) && (strncmp(string, extension, idx) == 0))
		{
			return true;
		}
		
		/** 当前串不是string,继续下一个字符串 */
		extension += (idx + 1);
	}
	/** 没有找到,则返回false */
	return false;   
}

/** 初始化 */
bool Test::initMultiTexture()
{
	/** 检查是否支持扩展 */
	if (isExtensionSupported("GL_ARB_multitexture"))
	{
				
		/** 获得函数指针地址 */
		glActiveTextureARB       = (PFNGLCLIENTACTIVETEXTUREARBPROC)wglGetProcAddress("glActiveTextureARB");
		glMultiTexCoord2fARB     = (PFNGLMULTITEXCOORD2FARBPROC)wglGetProcAddress("glMultiTexCoord2fARB");
		glClientActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)wglGetProcAddress("glClientActiveTextureARB");
		
		return true;
	}
	else 
		return false;
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
	ResizeDraw(true);	                             /**< 改变OpenGL窗口大小，直接调用子类的函数 */
	
	/** 启用纹理 */
	glEnable(GL_TEXTURE_2D);

	/** 检查是否支持多重纹理 */
	if(!initMultiTexture())
	{
		MessageBox(NULL,"不支持多重纹理!","错误",MB_OK);
	    exit(-1);
	}
	
	/** 载入纹理 */
	if(!m_texture.Load("woodfloor.tga"))
	{
		MessageBox(NULL,"载入纹理失败!","错误",MB_OK);
		exit(-1);
	}
	
	/** 指定材质 */
	float ambient_mat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	float diffuse_mat[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient_mat );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, diffuse_mat );

	/** 指定光源属性值 */
    float ambient_light0[]  = { 0.2f, 0.2f, 0.2f, 1.0f };
    float diffuse_light0[]  = { 1.0f, 1.0f, 1.0f, 1.0f };
	float position_light0[] = { 0.0f, 0.0f, 0.0f, 1.0f };
    float linearAttenuation_light0[] = { 0.5f };
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light0 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light0 );
    glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
    glLightfv( GL_LIGHT0, GL_LINEAR_ATTENUATION, linearAttenuation_light0 );
    
	/** 开启光照和光源 */
	glEnable(GL_LIGHTING);
	glEnable( GL_LIGHT0 );

	/** 设置环境光强度 */
	float ambient_lightModel[] = { 0.50f, 0.50f, 0.50f, 1.0f };
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_lightModel );
    
    /** 为每个顶点计算切向量 */
	computeTangentsAndBinormals();

	return true;                        /**< 成功返回 */
}

/** 计算dt方向切向量 */
Vector3 Test::computeTangentVector(Vertex pVtxA, Vertex pVtxB, Vertex pVtxC)
{

	Vector3 vAB = Vector3(pVtxB.x, pVtxB.y, pVtxB.z) - Vector3(pVtxA.x, pVtxA.y, pVtxA.z);
	Vector3 vAC = Vector3(pVtxC.x, pVtxC.y, pVtxC.z) - Vector3(pVtxA.x, pVtxA.y, pVtxA.z);
	Vector3 nA  = Vector3(pVtxA.nx, pVtxA.ny, pVtxA.nz);

    /** 计算用法向量的正交向量 */
    Vector3 vProjAB = vAB - ( nA * vAB.dotProduct(nA));
    Vector3 vProjAC = vAC - ( nA * vAC.dotProduct(nA));

    /** 计算纹理坐标u方向的差值 */
    float duAB = pVtxB.tu - pVtxA.tu;
    float duAC = pVtxC.tu - pVtxA.tu;

	/** 计算纹理坐标v方向的差值 */
    float dvAB = pVtxB.tv - pVtxA.tv;
    float dvAC = pVtxC.tv - pVtxA.tv;

    if( (duAC * dvAB) > (duAB * dvAC) )
    {
        duAC = -duAC;
        duAB = -duAB;
    }
    
	/** 计算切线量,并规一化 */
    Vector3 vTangent = (vProjAB * duAC) - (vProjAC * duAB);
    vTangent.normalize();
    return vTangent;
}

/** 计算每个顶点切线空间内的T和B分量 */
void Test::computeTangentsAndBinormals( void )
{	
    
	/** 将四边形分为两个三角形 */
    const int nNumIndices = 6;
	int indices[nNumIndices] = { 0,1,2,  2,3,1 };   /**< 三角形顶点索引 */

    /** 为每个面的各顶点计算切线量 */
    for( int i = 0; i < nNumIndices; i += 3 )
    {
		int a = indices[i+0];
        int b = indices[i+1];
        int c = indices[i+2];

        /** 将共享此顶点的切向量叠加 */
		g_vTangents[a] += computeTangentVector( g_quadVertices[a], g_quadVertices[b], g_quadVertices[c] );
	    g_vTangents[b] += computeTangentVector( g_quadVertices[b], g_quadVertices[a], g_quadVertices[c] );
	    g_vTangents[c] += computeTangentVector( g_quadVertices[c], g_quadVertices[a], g_quadVertices[b] );
	}

    /** 规一化T分量,并计算B分量 */
    for(int i = 0; i < NUM_VERTICES; ++i )
    {
		g_vTangents[i].normalize();
		
		Vector3 normal(g_quadVertices[i].nx, g_quadVertices[i].ny, g_quadVertices[i].nz);

		g_vBiNormals[i] = normal.crossProduct(  g_vTangents[i] );
    }
}

/** 偏移纹理坐标 */
void Test::shiftTextureCoordinates( void )
{
	/** 得到模型视点矩阵的逆矩阵 */
	CMatrix4x4 modelView;
    CMatrix4x4 inverseModelView;
    glGetFloatv( GL_MODELVIEW_MATRIX, &modelView.m[0] );
	inverseModelView = CMatrix4x4::invertMatrix( &modelView );

    /** 得到光源位置 */
    float fLightsPosition[4];
	glGetLightfv( GL_LIGHT0, GL_POSITION, fLightsPosition );
    Vector3 vLightsPosition( fLightsPosition[0], 
                              fLightsPosition[1], 
                              fLightsPosition[2] );
	
	/** 将光源位置转换到切空间内内 */
    inverseModelView.transformPoint( &vLightsPosition );

	/** 为每个顶点计算纹理坐标偏移量 */
    for( int i = 0; i < NUM_VERTICES; ++i )
    {
       
        Vector3 vLightToVertex;
        
		///计算从光源到顶点的向量
        vLightToVertex = vLightsPosition - Vector3( g_quadVertices[i].x, 
			                             g_quadVertices[i].y, g_quadVertices[i].z );
        ///规一化
        vLightToVertex.normalize();
        
		///同法向向量作点积
		Vector3 normal(g_quadVertices[i].nx, g_quadVertices[i].ny, g_quadVertices[i].nz);
       
		float r = normal.dotProduct( vLightToVertex );
        
        if( r < 0.0f )
        {

            /** 光源位于平面的下方时不进行偏移 */
            g_fEmbossTexCoords[i].tu2 = g_quadVertices[i].tu;
            g_fEmbossTexCoords[i].tv2 = g_quadVertices[i].tv;
        }
        else
        {
			/** 在切空间内扰动纹理坐标 */
            Vector3 vEmbossShift;
            
			vEmbossShift.x = vLightToVertex.dotProduct( g_vTangents[i] );
            vEmbossShift.y = vLightToVertex.dotProduct( g_vBiNormals[i] );
			
			vEmbossShift.normalize();  /**< 规一化 */
            
			g_fEmbossTexCoords[i].tu2 = g_quadVertices[i].tu + (vEmbossShift.x / 128) * g_fEmbossFactor;
            g_fEmbossTexCoords[i].tv2 = g_quadVertices[i].tv + (vEmbossShift.y / 128) * g_fEmbossFactor;
        }
    }
}

/** 渲染凹凸映射 */
void Test::renderQuadWithEmbossBumpMapping()
{
	/** 开启混合 */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ZERO);/**< 指定混合因子 */
	
	/** 纹理单元0 */
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture.ID);

	///指定纹理环境模式为组合函数方式。
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

	//指定RGB操作方式是乘法
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_MODULATE);
    
	///指定对颜色分量的缩放操作。这里是2。0f 乘以2加亮图像颜色
	glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 2.0f);

	///指定alpha的操作方式为替换方式
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_REPLACE);
	
	
	/** 纹理单元1 */
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, m_texture.ID);
	
	///指定纹理环境模式为组合函数方式
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_EXT);

	///指定RGB操作方式为替换
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_RGB_EXT, GL_REPLACE);

	/** 指定RGB缩放操作 */
	glTexEnvf(GL_TEXTURE_ENV, GL_RGB_SCALE_EXT, 1.0f);

	/** 将前面纹理的结果作为输入 */
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_RGB_EXT, GL_PREVIOUS_EXT);


	/** 指定alpha操作的方法 */
	glTexEnvf(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_EXT, GL_ADD_SIGNED_EXT);	
	glTexEnvf(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_EXT, GL_ONE_MINUS_SRC_ALPHA);
	
	/** 指定操作数一个是是纹理alpha ，另一个是先前的alpha */
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_EXT, GL_TEXTURE);
	glTexEnvf(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_EXT, GL_PREVIOUS_EXT);

	/** 开始渲染 */
	glBegin( GL_TRIANGLE_STRIP );
    {
		for( int i = 0; i < NUM_VERTICES; ++i )
        {
			glNormal3f( g_quadVertices[i].nx, g_quadVertices[i].ny, g_quadVertices[i].nz );
			
            ///指定纹理单元0的纹理坐标
			glMultiTexCoord2fARB( GL_TEXTURE0_ARB, g_quadVertices[i].tu, g_quadVertices[i].tv );

            ///指定纹理单元1的纹理坐标
            glMultiTexCoord2fARB( GL_TEXTURE1_ARB, g_fEmbossTexCoords[i].tu2, g_fEmbossTexCoords[i].tv2 );
			
			///指定顶点坐标
            glVertex3f( g_quadVertices[i].x, g_quadVertices[i].y, g_quadVertices[i].z );
		}
    }
	glEnd();

	
	///恢复状态,关闭两个纹理单元
	glActiveTextureARB(GL_TEXTURE1_ARB);
	glDisable(GL_TEXTURE_2D);
	glActiveTextureARB(GL_TEXTURE0_ARB);
	glDisable(GL_TEXTURE_2D);

	///关闭混合
	glDisable( GL_BLEND );   
}

/** 用户自定义的卸载函数 */
void Test::Uninit()									
{
	m_texture.FreeImage();
	glDeleteTextures(1,&m_texture.ID);
}

/** 程序更新函数 */
void Test::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

/** 用户自定义的更新过程 */
	
	
	/** 空格键切换绘制模式 */
	if(m_Keys.IsPressed(VK_SPACE) && !sp)
	{
		sp = true;
		g_bEmbossBumpMap = !g_bEmbossBumpMap;
	}

	if(!m_Keys.IsPressed(VK_SPACE))
		sp = false;
	
	/** 更新光源位置 */
	static float fAngle = 0; 
	fAngle += 60 * (float)milliseconds*0.001;

	/** 角度控制 */
	while(fAngle > 360.0f) 
		fAngle -= 360.0f;
	while(fAngle < 0.0f)  
		fAngle += 360.0f;

    float x = sinf( DEGTORAD(fAngle) );
    float y = cosf( DEGTORAD(fAngle) );

    float position_light0[4];

    position_light0[0] = 2.0f * x;
    position_light0[1] = 2.0f * y;
    position_light0[2] = 0.0f;
    position_light0[3] = 1.0f;

	/** 指定光源位置 */
    glLightfv( GL_LIGHT0, GL_POSITION, position_light0 );
	    
}


/** 绘制函数 */
void Test::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	
	
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	glTranslatef(0.0f,0.0f,-4.0f);

	
	if( g_bEmbossBumpMap == true )
    {
        /** 扰动纹理坐标 */
        shiftTextureCoordinates();

        /** 渲染凹凸映射 */
	    renderQuadWithEmbossBumpMapping();
    }
    else
    {
        /** 不进行凹凸映射渲染 */
		glDisable( GL_BLEND );

		/** 启用纹理单元0 */
		glActiveTextureARB( GL_TEXTURE0_ARB );
		glEnable( GL_TEXTURE_2D );
		glBindTexture( GL_TEXTURE_2D, m_texture.ID );

		glBegin( GL_TRIANGLE_STRIP );
        {
		    for( int i = 0; i < 4; i++ )
            {
			    glNormal3f( g_quadVertices[i].nx, g_quadVertices[i].ny, g_quadVertices[i].nz );
                glTexCoord2f( g_quadVertices[i].tu, g_quadVertices[i].tv );
			    glVertex3f( g_quadVertices[i].x, g_quadVertices[i].y, g_quadVertices[i].z );
		    }
        }
	    glEnd();
    }
	glPopMatrix();        /**< 绘制结束 */


	/** 绘制一个小球代表光源 */
	glDisable( GL_LIGHTING );     /**< 关闭光照 */
    glDisable(GL_TEXTURE_2D);     /**< 关闭纹理 */
 	glColor3f( 1.0f, 1.0f, 1.0f );
   
	glMatrixMode( GL_MODELVIEW );
	glPushMatrix();
	glLoadIdentity();

	/** 得到光源位置 */
	float fLightsPosition[4];
	glGetLightfv( GL_LIGHT0, GL_POSITION, fLightsPosition );
	glTranslatef( fLightsPosition[0], fLightsPosition[1], -5.0f );
	auxSolidSphere(0.1);
	glPopMatrix();

	/** 启用光照和纹理 */
	glEnable( GL_LIGHTING );
	glEnable(GL_TEXTURE_2D);

	glFlush();	                 /**< 强制执行所有的OpenGL命令 */
}
