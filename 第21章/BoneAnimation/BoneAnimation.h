//=========================================================================
/**
*  @file      BoneAnimation.h
*
*  项目描述： 骨骼动画
*  文件描述:  具体实例类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-09-13	
*  修改日期： 2006-12-29
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

#ifndef __BoneAnimation_H__
#define __BoneAnimation_H__

#include "stdafx.h"

#include "GLFrame.h"            /**< 包含基本的框架类 */
#include "Vector.h"
#include "Bone.h"	

/** 定义一些变量 */
#define MAX_BONES                2
#define MAX_MESHES               3
#define MAX_MESH_POINTS          4

/** 从GL_Application派生出一个子类 */
class Test : GLApplication								
{
public:
	bool	Init();							   /**< 执行所有的初始化工作，如果成功函数返回true */
	void	Uninit();						   /**< 执行所有的卸载工作 */
	void	Update(DWORD milliseconds);		   /**< 执行所有的更新操作，传入的参数为两次操作经过的时间，以毫秒为单位 */
	void	Draw();							   /**< 执行所有的绘制操作 */

	/** 更新骨骼 */
	void    UpdateBones();
	
		
private:
	friend class GLApplication;				   /**< 父类为它的一个友元类 */
	Test(const char * class_name);	           /**< 构造函数 */

	/** 用户自定义的程序变量 */
	CVector4      trans;                       /**< 用于平移骨骼模型 */   
	CBone         bones[MAX_BONES];            /**< 保存骨骼结构 */
	CMatrix4x4    displayBones[MAX_BONES];     /**< 用于绘制骨骼 */
	CBoneVertex   modelPoints[MAX_MESHES * MAX_MESH_POINTS]; /**< 模型顶点数据 */
		
};


#endif	// __BoneAnimation_H__