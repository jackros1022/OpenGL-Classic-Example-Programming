//========================================================
/**
*  @file      Terrain.h
*
*  项目描述： TGA文件载入
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

#ifndef _TERRAIN_H
#define _TERRAIN_H


#include "stdafx.h"
#include "CBMPLoader.h"

/** 地形类 */
class CTerrain
{
public:
	/** 构造函数 */
	CTerrain();
	~CTerrain();
    
	/** 设置地形的大小 */
	void setSize(unsigned  int width, unsigned  int cell); 
	
	/** 初始化地形 */
	bool initTerrain();

	/** 载入'.raw'高度图 */
	bool loadRawFile(const char* fileName);

	/** 装载地形纹理 */
	bool loadTexture(const char * fileName);

	/** 获得点(x,y)的高度信息 */
	int getHeight(int x, int y);

	/** 获得地面高度 */
	float getAveHeight(float x, float z);

	/** 设置纹理坐标 */
	void setTexCoord(float x, float z);

	/** 渲染地形 */
	void render();
	
private:
	unsigned  int   m_nWidth;          /**< 地形网格数 */
	unsigned  int   m_nCellWidth;      /**< 每一格宽度 */
   	BYTE*           m_pHeightMap;      /**< 存放高度信息 */
	CBMPLoader      m_texture;         /**< 载入纹理 */
	
};

#endif

