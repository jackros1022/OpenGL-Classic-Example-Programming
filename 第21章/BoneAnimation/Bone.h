//========================================================
/**
*  @file      Bone.h
*
*  项目描述： 骨骼类
*  文件描述:  骨骼动画    
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-10-13	
*  修改日期： 2006-12-29
*
*/     
//========================================================
#ifndef __BONE_H__
#define __BONE_H__

#include "Matrix.h"

/** 骨骼类 */
class CBone
{
public:
      CBone();
      
	  /** 设置骨骼 */
      void SetBone(unsigned int P, float L);
      void SetBone(unsigned int P, float L, CMatrix4x4 R, CMatrix4x4 A);

      unsigned int parent;  /**< 父结点 */
      float length;         /**< 长度 */

	  /** 变换矩阵 */
      CMatrix4x4 relative; 
      CMatrix4x4 absolute;
};

/** 骨骼顶点类 */
class CBoneVertex
{
   public:
      CBoneVertex();

	  /** 设置顶点数据 */
      void SetVertex(CVector4 p, CVector4 n, int b1, int b2, int b3, int b4,
                     float w1, float w2, float w3, float w4, float R, float G,
                     float B, float A, int numB);
      
      
      CVector4 pos;         /**< 位置 */
      CVector4 normal;      /**< 法线 */
      
      int boneIndex[4];     /**< 骨骼索引 */
      float weights[4];     /**< 权值 */
      int numBones;         /**< 影响的骨骼数目 */

      float r, g, b, a;     /**< 颜色 */
};

#endif