//=========================================================================
/**
*  @file      Test.h
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

#ifndef __TEST_H__
#define __TEST_H__

#include "stdafx.h"
#include "GLFrame.h"	     /**< 包含基本的框架类 */
#include "TGALoader.h"
#include "Vector.h"
#include "Matrix.h"
#include "glext.h"           /**< 包含glext.h头文件 */

/** 定义顶点结构体 */
struct Vertex
{
    float tu, tv;     /**< 纹理坐标 */
    float nx, ny, nz; /**< 法线 */
    float x, y, z;    /**< 顶点坐标 */
};

/** 顶点个数 */
const int NUM_VERTICES = 4;

/** 纹理坐标结构 */
struct TexCoords
{
    float tu2, tv2;
};

/** 从GL_Application派生出一个子类 */
class Test : GLApplication								
{
public:
	bool	Init();							   /**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						   /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		   /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							   /**< 执行所有的绘制操作 */
	
	bool    isExtensionSupported(const char *string);  /**< 检查是否支持扩展 */
	bool    initMultiTexture();                        /**< 初始化 */
	
    Vector3 computeTangentVector(Vertex pVtxA,Vertex pVtxB,Vertex pVtxC); /**< 计算切线量 */
    void computeTangentsAndBinormals(void);     /**< 计算所有顶点的切线T分量和B分量 */
    void shiftTextureCoordinates(void);         /**< 偏移纹理坐标 */
    void renderQuadWithEmbossBumpMapping(void); /**< 渲染浮雕凹凸纹理 */

			
private:
	friend class GLApplication;				  
	Test(const char * class_name);	       /**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	CTGALoader  m_texture;                   /**< TGA文件载入类 */
	bool        sp;                          /**< 空格键是否释放 */

};


#endif	// __TEST_H__