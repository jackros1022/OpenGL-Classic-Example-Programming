//========================================================
/**
*  @file      SkyBox.h
*
*  项目描述： TGA文件载入
*  文件描述:  天空盒类  
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-12-06	
*  修改日期： 2006-12-10
*
*/     
//========================================================
#ifndef __SKYBOX_H__
#define __SKYBOX_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "Vector.h"
#include "Camera.h"

#define GL_CLAMP_TO_EDGE	0x812F

/** 天空盒类 */
class CSkyBox
{
public:
	/** 构造函数 */
	CSkyBox();
	~CSkyBox();

	/** 初始化 */
	bool Init();
	
	/** 渲染天空 */
	void  CreateSkyBox(float x, float y, float z, 
		               float width, float height, 
					   float length);

private:
	
	CBMPLoader  m_texture[6];   /**< 天空盒纹理 */
		
};


#endif ///__SKYBOX_H__