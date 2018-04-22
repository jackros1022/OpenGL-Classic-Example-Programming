//=========================================================================
/**
*  @file      ShadowVolume.h
*
*  项目描述： 阴影体
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-20
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

#ifndef __SHADOW_VOLUME_H__
#define __SHADOW_VOLUME_H__

#include "stdafx.h"

#include "GLFrame.h"												/**< 包含基本的框架类 */
#include "Font.h"
#include "Vector.h"
#include "TGALoader.h"

/** 从GL_Application派生出一个子类 */
class Test : GLApplication								
{
public:
	bool	Init();							   /**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						   /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		   /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							   /**< 执行所有的绘制操作 */
	
	void    CaculateFrameRate();               /**< 计算帧速 */
	void    PrintText();                       /**< 输出文字信息 */
  
	/** 绘制场景中所有物体 */
	void    render();    

	/** 绘制阴影体 */
	void    DrawSilhouette(CVector4 lightPos, float Extend);
	
private:
	friend class GLApplication;				   /**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	Test(const char * class_name);	           /**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	CTGALoader  ground;                        /** 地面纹理 */
 	GLFont      m_Font;                        /**< 字体类 */
  	float       m_Fps;                         /**< 帧速 */
	bool        sp;                            /**< 判断空格键是否释放 */

};


#endif	// __SHADOW_VOLUME_H__