//=========================================================================
/**
*  @file      Tess.h
*
*  项目描述： OpenGL网格化
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-02
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

#ifndef __TESS_H__
#define __TESS_H__


#include "GLFrame.h"												/**< 包含基本的框架类 */
#include <gl\gl.h>												    /**< 包含OpenGL头文件 */
#include <gl\glu.h>
#include "Font.h"

/** 从GL_Application派生出一个子类 */
class Tess : GLApplication								
{
public:
	bool	Init();							/**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						/**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		/**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							/**< 执行所有的绘制操作 */

	bool    InitTess();                     /**< 初始化网格化对象 */

private:
	friend class GLApplication;				/**< 父类为它的一个友元类，可以用来创建程序的实例，见函数GL_Application * GL_Application::Create(const char * class_name) */
	Tess(const char * class_name);		    /**< 构造函数 */
/** 用户自定义的程序变量 */ 
	GLFont font;                            /**< 字体 */                  
	GLUtesselator *tobj;                    /**< 网格化对象指针 */                
  	GLfloat m_Angle;                        /**< 旋转角度 */
	GLuint list;                            /**< 显示列表索引 */
	bool   wp;                              /**< 判断W键是否被释放 */
	bool   sp;								/**< 判断空格键是否被释放 */
	
};


#endif	// __TESS_H__