//========================================================
/**
*  @file      Terrain.cpp
*
*  项目描述： 构造天空和地面
*  文件描述:  地形类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-12-06	
*  修改日期： 2006-12-10
*
*/     
//========================================================

#include "Terrain.h"
#include "CBMPLoader.h"

/** 构造函数 */
CTerrain::CTerrain()//:m_vertex(0),m_index(0),m_texCoord(0)
{
	/** 设置地形大小 */
	setSize(MAP_WIDTH,CELL_WIDTH);
	
	/** 为地形高程分配内存,并初始化 */
	m_pHeightMap = new BYTE[m_nWidth * m_nWidth];
	for(int i=0; i<m_nWidth * m_nWidth; i++)
		m_pHeightMap[i] = 0;
}

/** 析构函数 */
CTerrain::~CTerrain()
{
	/** 删除内存和指针 */
	if(m_pHeightMap)
	{
		delete[] m_pHeightMap;
	    m_pHeightMap = 0;
	}
	/** 删除纹理对象及其占用内存 */	
	m_texture.FreeImage();
	glDeleteTextures(1,&m_texture.ID);
		
}

/** 初始化地形 */
bool CTerrain::initTerrain()
{
	/** 载入高度文件 */
	loadRawFile("data/terrain.raw");

	/** 载入纹理 */
	loadTexture("data/terrain.bmp");

	return true;
}

/** 设置地形大小 */
void CTerrain::setSize(unsigned  int width, unsigned  int cell)
{
	m_nWidth = width;
	m_nCellWidth = cell; 
}

/** 获得高度图中高度值 */
int CTerrain::getHeight(int x,int y)
{
	if(!m_pHeightMap)
		return 0;
	int xx = x % m_nWidth;
	int yy = y % m_nWidth;
    /** 返回当前高度 */
	return m_pHeightMap[(xx + yy * m_nWidth)];
}
/** 获得地面高度 */
float CTerrain::getAveHeight(float x,float z)
{
	float CameraX, CameraZ;

	CameraX = x / m_nCellWidth;
	CameraZ = z / m_nCellWidth;
	/** 计算高程坐标(Col0, Row0)，(Col1, Row1) */
	int col0 = int(CameraX);
	int row0 = int(CameraZ);
	unsigned int col1 = col0 + 1;
	unsigned int row1 = row0 + 1;
	/** 确保单元坐标不超过高程以外 */
	if (col1 > m_nWidth) col1 = 0;
	if (row1 > m_nWidth) row1 = 0;

	/** 获取单元的四个角的高度 */
	float h00 = (float)(m_pHeightMap[col0*m_nCellWidth + row0*m_nCellWidth*m_nWidth]);
	float h01 = (float)(m_pHeightMap[col1*m_nCellWidth + row0*m_nCellWidth*m_nWidth]);
	float h11 = (float)(m_pHeightMap[col1*m_nCellWidth + row1*m_nCellWidth*m_nWidth]);
	float h10 = (float)(m_pHeightMap[col0*m_nCellWidth + row1*m_nCellWidth*m_nWidth]);

	/** 计算机摄像机相对于单元格的位置 */
	float tx = CameraX - int(CameraX);
	float ty = CameraZ - int(CameraZ);

	/** 进行双线性插值得到地面高度 */
	float txty = tx * ty;

	float final_height	= h00 * (1.0f - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);
	return final_height;
}

/** 载入高度图 */
bool CTerrain::loadRawFile(const char* fileName)
{
	FILE *pFile = NULL;

	/** 打开文件 */
	pFile = fopen( fileName, "rb" );

	/** 错误检查 */
	if ( pFile == NULL )	
	{
		/** 输出错误信息，并返回false */
		MessageBox(NULL, "打开高度图文件失败！", "错误", MB_OK);
		exit(0);
	}

	/** 读取高度图文件 */
	fread( m_pHeightMap, 1, m_nWidth*m_nWidth, pFile );

	/** 获取错误代码 */
	int result = ferror( pFile );

	/** 检查错误代码 */
	if (result)
	{
		MessageBox(NULL, "无法获取高度数据！", "错误", MB_OK);
		exit(0);
	}
    /** 关闭文件，成功返回 */
	fclose(pFile);
	return true;
}

/** 设置纹理坐标 */
void CTerrain::setTexCoord(float x,float z)
{
	glTexCoord2f(   (float)x / (float)m_nWidth,	
				  - (float)z / (float)m_nWidth	);
}

/** 载入地面纹理 */
bool CTerrain::loadTexture(const char* fileName)
{
	if(!m_texture.LoadBitmap(fileName))                   /**< 载入位图文件 */
	{
		MessageBox(NULL,"装载位图文件失败！","错误",MB_OK);    /**< 如果载入失败则弹出对话框 */
		exit(0);
	}
	glGenTextures(1, &m_texture.ID);                            /**< 生成一个纹理对象名称 */
		
	glBindTexture(GL_TEXTURE_2D, m_texture.ID);                 /**< 创建纹理对象 */
	/** 控制滤波 */
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
	/** 创建纹理 */
	gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, m_texture.imageWidth,
					m_texture.imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					m_texture.image);
  	return true;
}

/** 渲染地形 */
void CTerrain::render()
{
		
	
	int X = 0, Y = 0;						
	float x, y, z;							
	bool bSwitchSides = false;

	/** 检查高度图是否有效 */
	if(!m_pHeightMap) 
		return;					

	/** 绑定纹理 */
	glBindTexture(GL_TEXTURE_2D, m_texture.ID);
	glDisable(GL_LIGHTING);                          /** 关闭光照 */

	/** 绘制三角形带 */
	glBegin( GL_TRIANGLE_STRIP );			

	/** 从行(X)开始循环 */
	for ( X = 0; X <= m_nWidth; X += m_nCellWidth )
	{
		/** 检查该列是否需要从相反顺序绘制 */
		if(bSwitchSides)
		{	
			/** 绘制地形的一列 */
			for ( Y = m_nWidth; Y >= 0; Y -= m_nCellWidth )
			{
				/** 顶点(X, Y, Z) */		
				x = X;							
				y = getHeight( X, Y );	
				z = Y;							

				/** 指定纹理坐标,并发送顶点 */
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);		

				/** 顶点(X + m_nCellWidth, Y, Z) */		
				x = X + m_nCellWidth; 
				y = getHeight( X + m_nCellWidth, Y ); 
				z = Y;

				/** 指定纹理坐标,并发送顶点 */
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);			
			}
		}
		else
		{	
			/** 绘制地形的一列 */
			for ( Y = 0; Y <= m_nWidth; Y += m_nCellWidth )
			{
				/** 顶点(X + m_nCellWidth, Y, Z) */	
				x = X + m_nCellWidth; 
				y = getHeight( X + m_nCellWidth, Y ); 
				z = Y;

				/** 指定纹理坐标,并发送顶点 */
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);

				/** 顶点 (X, Y, Z) */		
				x = X;							
				y = getHeight( X, Y );	
				z = Y;							

				/** 指定纹理坐标,并发送顶点 */
				setTexCoord( (float)x, (float)z );
				glVertex3f(x, y, z);		
			}
		}
			
		/** 变换开关 */
		bSwitchSides = !bSwitchSides;
	}

	/** 绘制结束 */
	glEnd();

}

