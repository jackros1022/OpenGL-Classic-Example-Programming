//=========================================================================
/**
*  @file      Bezier.h
*
*  项目描述： Bezier曲面
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-04
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

#ifndef __BEZIER_H__
#define __BEZIER_H__


#include "GLFrame.h"            /**< 包含基本的框架类 */

#include <gl\gl.h>	            /**< 包含OpenGL头文件 */
#include <gl\glu.h>	
#include "CBMPLoader.h"         /**< 包含载入位图头文件 */
#include "Font.h"               /**< 字体类头文件 */

/** 从GL_Application派生出一个子类 */
class Bezier : GLApplication								
{
public:
	bool	Init();										/**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();								    /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);				    /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();									    /**< 执行所有的绘制操作 */

	bool    LoadTexture();                              /**< 载入纹理 */
private:
	friend class GLApplication;							/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	Bezier(const char * class_name);					/**< 构造函数 */

	/** 用户自定义的程序变量 */
    GLfloat      rot;                                   /**< 用于旋转物体 */
	CBMPLoader   Texture;                               /**< 载入位图类对象 */
	GLFont       font;                                  /**< 字体类 */
	GLboolean    light;                                 /**< 用于开启/关闭光源 */
	GLboolean    lp;                                    /**< 判断L键是否释放 */
	GLboolean    sp;                                    /**< 判断空格键是否释放 */
	                            
		
};

#endif	// __BEZIER_H__