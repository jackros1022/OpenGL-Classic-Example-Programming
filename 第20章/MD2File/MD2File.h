//=========================================================================
/**
*  @file      Test.h
*
*  项目描述： MD2文件读取
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-11
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
#include "BMPLoader.h"
#include "GLFrame.h"												/**< 包含基本的框架类 */
#include "Font.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "MD2Loader.h"



/** 从GL_Application派生出一个子类 */
class Test : GLApplication								
{
public:
	bool	Init();							   /**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						   /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		   /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							   /**< 执行所有的绘制操作 */
	
	
	void    UpdateCamera();                    /**< 更新摄像机 */
	void    CaculateFrameRate();               /**< 计算帧速 */
	void    PrintText();                       /**< 输出文字信息 */
		
	void    Animate(float x,float z,
		            float h,float scale);      /**< 播放MD2动画 */
private:
	friend class GLApplication;				   
	Test(const char * class_name);	      /**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	CMD2Loader m_MD2;                         /**< MD2载入类 */
	CTerrain   m_Terrain;                     /**< 地形类 */
	CSkyBox    m_SkyBox;                      /**< 天空类 */
	GLFont     m_Font;                        /**< 字体类 */
	Camera     m_Camera;                      /**< 摄像机类 */     
  	float      m_Fps;                         /**< 帧速 */
	bool       m_RenderMode;		          /**< 绘制模式 */
	bool       sp;                            /**< 空格键是否释放 */
	bool       mp;                            /**< 'M'键是否释放 */
};

#endif	// __TEST_H__