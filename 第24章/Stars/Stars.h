//=========================================================================
/**
*  @file      Protechny.h
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

#ifndef __TEST_H__
#define __TEST_H__

#include "stdafx.h"
#include "BMPLoader.h"
#include "Font.h" 
#include "GLFrame.h"	
#include "Particle.h"


/** 从GL_Application派生出一个子类 */
class Test : GLApplication								
{
public:
	bool	Init();							   /**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						   /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		   /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							   /**< 执行所有的绘制操作 */
	
	void    DrawParticle();                    /**< 绘制粒子 */
		
	bool    InitStars();                   /**< 初始化喷泉 */
	void    UpdateStars();                 /**< 更新喷泉 */
	
	void    PrintText();                       /**< 输出信息 */
	void    CaculateFrameRate();               /**< 计算帧速 */

private:
	friend class GLApplication;				   
	Test(const char * class_name);	          /**< 构造函数 */

	/** 用户自定义的程序变量 */ 
	
	CParticle*       Stars;        /**< 粒子系统的对象 */
   	GLFont           m_Font;          /**< 字体类 */
	float            m_Fps;           /**< 保存帧速 */
	float            angle;           /**< 地面的旋转角度 */
			
};

#endif	// __TEST_H__