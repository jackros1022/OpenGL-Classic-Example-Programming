
#ifndef __MATRIX_H__
#define __MATRIX_H__

#include "Vector.h"
#include <memory.h>
#include <math.h>

//using namespace std;

/** 弧度和角度之间转换 */
#define DEGTORAD(degree) ((degree) * (3.141592654f / 180.0f))
#define RADTODEG(radian) ((radian) * (180.0f / 3.141592654f))


/** 矩阵类(4x4) */
class CMatrix4x4
{
public:

	/** 构造函数 */
	CMatrix4x4()  { identity(); }

	CMatrix4x4( float m0, float m4, float  m8, float m12,
                float m1, float m5, float  m9, float m13,
                float m2, float m6, float m10, float m14,
                float m3, float m7, float m11, float m15 );

	/** 初始化为单位化矩阵 */
	void identity(void);

	/** 平移 */
	void translate(const Vector3 &trans);
	void translate_x(const float &dist);
	void translate_y(const float &dist);
	void translate_z(const float &dist);

	/** 旋转 */
	void rotate(const float &angle, Vector3 &axis);
	void rotate_x(const float &angle);
	void rotate_y(const float &angle);
	void rotate_z(const float &angle);
	void scale(const Vector3 &scale);
    void transformPoint( Vector3 *vec );
    void transformVector( Vector3 *vec );

	
	static CMatrix4x4 CMatrix4x4::invertMatrix( const CMatrix4x4 *matIn );
	
    /** 操作符重载 */
	CMatrix4x4 operator + (const CMatrix4x4 &other);
	CMatrix4x4 operator - (const CMatrix4x4 &other);
	CMatrix4x4 operator * (const CMatrix4x4 &other);

	CMatrix4x4 operator * (const float scalar);

public:
	
	float m[16];
};



#endif // __MATRIX_H__

