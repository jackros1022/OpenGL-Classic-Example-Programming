//=========================================================================
/**
*  @file  3DGraphics.h
*
*  项目描述： 3D图形的绘制
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-10-13	
*  修改日期： 2006-11-13
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

#ifndef __3D_GRAPHICS_H__
#define __3D_GRAPHICS_H__


#include "GLFrame.h"												/**< 包含基本的框架类 */

/** 从GL_Application派生出一个子类 */
class Graphics : GLApplication								
{
public:
	bool	Init();										/**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();										/**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);							/**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();												/**< 执行所有的绘制操作 */

private:
	friend class GLApplication;								/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	Graphics(const char * class_name);						/**< 构造函数 */
/** 用户自定义的程序变量 */
	float	m_Angle;											/**< 使用它来旋转图形 */
	int m_Pivot;                                                /**< 用于设置圆锥间隔颜色 */
	
	
};


#endif	// __3D_GRAPHICS_H__