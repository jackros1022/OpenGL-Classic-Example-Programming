//=========================================================================
/**
*  @file      TGAFile.h
*
*  项目描述： TGA文件读取
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

#ifndef __TGAFILE_H__
#define __TGAFILE_H__

#include "stdafx.h"
#include "CBMPLoader.h"
#include "GLFrame.h"												/**< 包含基本的框架类 */
#include "Font.h"
#include "Camera.h"
#include "SkyBox.h"
#include "Terrain.h"
#include "TGALoader.h"


/** 从GL_Application派生出一个子类 */
class TGAFile : GLApplication								
{
public:
	bool	Init();							   /**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						   /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		   /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							   /**< 执行所有的绘制操作 */
	
	
	void    UpdateCamera();                    /**< 更新摄像机 */
	void    CaculateFrameRate();               /**< 计算帧速 */
	void    PrintText();                       /**< 输出文字信息 */
	void    DrawSphere(float x,float z,        /**< 绘制球体 */
		               float radius); 
	void    DrawBox(float x,float z,           /**< 绘制木箱 */
		               float length);
		
private:
	friend class GLApplication;				   /**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	TGAFile(const char * class_name);	   /**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	CTGALoader m_TGATexture[2];               /**< TGA文件载入类 */
	CTerrain   m_Terrain;                     /**< 地形类 */
	CSkyBox    m_SkyBox;                      /**< 天空类 */
	GLFont     m_Font;                        /**< 字体类 */
	Camera     m_Camera;                      /**< 摄像机类 */     
  	float      m_Fps;                         /**< 帧速 */
	bool       m_RenderMode;		          /**< 绘制模式 */
	bool       sp;                            /**< 空格键是否释放 */
};


#endif	// __TGAFILE_H__