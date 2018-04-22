//========================================================
/**
*  @file      Vector.h
*
*  项目描述： 向量类
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

#ifndef __VECTOR_H__
#define __VECTOR_H__

#include <math.h>
#define M_PI 3.141592654

/** 四维向量类 */
class CVector4
{
	public:
	  ///构造函数
      CVector4();                                        
      CVector4(float X, float Y, float Z);              
      CVector4(float X, float Y, float Z, float W);      
      CVector4(const CVector4 &v);                       

      ///运算符重载 
	  void operator=(CVector4 v);                        
      CVector4 operator-(CVector4 v);                   
      CVector4 operator+(CVector4 v);                    
      CVector4 operator*(CVector4 v);                   
      CVector4 operator/(CVector4 v);                   
      CVector4 operator+(float f);                       
      CVector4 operator-(float f);                       
      CVector4 operator*(float f);                       
      CVector4 operator/(float f);                      

      void operator +=(CVector4 v);                      
      void operator -=(CVector4 v);                      
      void operator *=(CVector4 v);                     
      void operator /=(CVector4 v);                      
      void operator +=(float f);                         
      void operator -=(float f);                        
      void operator *=(float f);                         
      void operator /=(float f);                        

      bool operator ==(CVector4 v);                    
      bool operator !=(CVector4 v); 

	  ///叉乘
	  void CrossProduct(CVector4 v1, CVector4 v2);       
      void CrossProduct3(CVector4 v1, CVector4 v2,
                         CVector4 v3);  

	  ///点乘
      float DotProduct3(CVector4 v1);                   
      float DotProduct4(CVector4 v1);  

	  ///返回向量长度
      float GetLength();   

	  ///归一化向量
      void Normal();                                     

public:
	   float x, y, z, w; 
};


#endif