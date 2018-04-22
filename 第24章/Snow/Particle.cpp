//======================================================================
/**
*  @file      Particle.cpp
*
*  项目描述： 粒子系统
*  文件描述:  粒子定义类 
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-12-18	
*  修改日期： 2006-12-19
*	
*/																			
//======================================================================
#include "Particle.h"

/** 构造函数 */
CParticle::CParticle()
{
	data = NULL;
	numparticle = 0;
	
}

/** 析构函数 */
CParticle::~CParticle()
{
	delete []data;
	data = NULL;
}

/** 创建一个包含num个元素的粒子数组 */
int CParticle::Create(long num)
{
	/** 删除粒子数组 */
	if (data) 
		delete []data;

	/** 创建数组 */
	if(data = new Particle[num])
	{
		memset(data,0,sizeof(Particle)*numparticle);
		numparticle = num;
		
		/** 返回粒子个数 */
		return numparticle;      
	}
	return 0;
}



/** 载入纹理 */
bool CParticle::LoadTextures()
{
	
	char* fileName[] = {"Data/wall.bmp","Data/snowball.bmp" };
	
	for(int i=0; i<2; i++)
	{
		if(!texture[i].LoadBitmap(fileName[i]))
		{
			MessageBox(NULL,"载入纹理失败!","错误",MB_OK);
			exit(-1);
		}

		glGenTextures(1,&texture[i].ID);         
		
		/** 创建纹理对象 */
		glBindTexture(GL_TEXTURE_2D, texture[i].ID);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);

		gluBuild2DMipmaps(GL_TEXTURE_2D, GL_RGB, texture[i].imageWidth,
						texture[i].imageHeight, GL_RGB, GL_UNSIGNED_BYTE,
					    texture[i].image);
	}
	return true;
		
}

/** 返回纹理ID */
unsigned int CParticle::GetTexture(int index)
{
	if(index == 0 || index == 1)
		
		return texture[index].ID; 
}

/** 设置和获取颜色Color的函数实现 */
int CParticle::SetColor(GLint r,GLint g,GLint b)
{
	for (int index=0;index<numparticle;++index)
	{
		data[index].r=r;
		data[index].g=g;
		data[index].b=b;
	}
	return TRUE;
}

int CParticle::SetColor(GLint index,GLint r,GLint g,GLint b)
{
	if(index>=0 && index<numparticle)
	{
		data[index].r=r;
		data[index].g=g;
		data[index].b=b;
		return TRUE;
	}
	return FALSE;
}

int CParticle::GetColor(GLint index,GLint &r,GLint &g,GLint &b)
{
	if(index>=0 && index<numparticle)
	{
		r=data[index].r;
		g=data[index].g;
		b=data[index].b;
		return TRUE;
	}
	return FALSE;
}

/** 设置和获取位置Position的函数实现 */
int CParticle::SetPosition(GLfloat x,GLfloat y,GLfloat z)
{
	for(int index=0;index<numparticle;++index)
	{
		data[index].x=x;
		data[index].y=y;
		data[index].z=z;
	}
	return TRUE;
}

int CParticle::SetPosition(GLint index,GLfloat x,GLfloat y,GLfloat z)
{
	if(index>=0 && index<numparticle)
	{
		data[index].x=x;
		data[index].y=y;
		data[index].z=z;
		return TRUE;
	}
	return FALSE;
}

int CParticle::GetPosition(GLint index,GLfloat &x,GLfloat &y,GLfloat &z)
{
	if(index>=0 && index<numparticle)
	{
		x=data[index].x;
		y=data[index].y;
		z=data[index].z;
		return TRUE;
	}
	return FALSE;
}
/** 设置和获取加速度Acceleration的函数实现 */
int CParticle::SetAcceleration(GLfloat ax,GLfloat ay,GLfloat az)
{
	for (int index=0;index<numparticle;++index)
	{
		data[index].ax=ax;
		data[index].ay=ay;
		data[index].az=az;
	}
	return TRUE;
}

int CParticle::SetAcceleration(GLint index,GLfloat ax,GLfloat ay,GLfloat az)
{
	if(index>=0 && index<numparticle)
	{
		data[index].ax=ax;
		data[index].ay=ay;
		data[index].az=az;
		return TRUE;
	}
	return FALSE;
}


int CParticle::GetAcceletation(GLint index,GLfloat &ax,GLfloat &ay,GLfloat &az)
{
	if(index>=0 && index<numparticle)
	{
		ax=data[index].ax;
		ay=data[index].ay;
		az=data[index].az;
		return TRUE;
	}
	return FALSE;
}



/** Velocity函数的实现 */
int CParticle::SetVelocity(GLfloat vx,GLfloat vy,GLfloat vz)
{
	for (int index=0;index<numparticle;++index)
	{
		data[index].vx=vx;
		data[index].vy=vy;
		data[index].vz=vz;
	}
	return TRUE;
}

int CParticle::SetVelocity(GLint index,GLfloat vx,GLfloat vy,GLfloat vz)
{
	if(index>=0 && index<numparticle)
	{
		data[index].vx=vx;
		data[index].vy=vy;
		data[index].vz=vz;
		return TRUE;
	}
	return FALSE;
}

int CParticle::GetVelocity(GLint index,GLfloat &vx,GLfloat &vy,GLfloat &vz)
{
	if(index>=0 && index<numparticle)
	{
		vx=data[index].vx;
		vy=data[index].vy;
		vz=data[index].vz;
		return TRUE;
	}
	return FALSE;
}


/** Size函数的实现 */
int CParticle::SetSize(GLfloat size)
{
	for (int index=0;index<numparticle;++index)
	{
		data[index].size=size;
	}
	return TRUE;
}

int CParticle::SetSize(GLint index,GLfloat size)
{
	if (index>=0 && index<numparticle)
	{
		data[index].size=size;
		return TRUE;
	}
	return FALSE;
}

int CParticle::GetSize(GLint index,GLfloat &size)
{
	if(index >= 0 && index < numparticle)
	{
		size=data[index].size;
		return TRUE;
	}
	return FALSE;
}

/** 消失速度Dec函数 */
int CParticle::SetDec(GLfloat dec)
{
	for (int index=0;index<numparticle;++index)
	{
		data[index].dec=dec;
	}
	return TRUE;
}
int CParticle::SetDec(GLint index,GLfloat dec)
{
	if(index >= 0 && index < numparticle)
	{
		data[index].dec=dec;
		return TRUE;
	}
	return FALSE;
}
int CParticle::GetDec(GLint index,GLfloat &dec)
{
	if(index >= 0 && index < numparticle)
	{
		dec=data[index].dec;
		return TRUE;
	}
	return FALSE;
}

/** 设置粒子的lifetime 属性 */
int CParticle::SetLifeTime(GLfloat lifetime)
{
	for (int index=0;index<numparticle;++index)
	{
		data[index].lifetime=lifetime;
	}
	return TRUE;
}
int CParticle::SetLifeTime(GLint index,GLfloat lifetime)
{
	if(index >= 0 && index < numparticle)
	{
		data[index].lifetime=lifetime;
		return TRUE;
	}
	return FALSE;
}

/** 获得粒子的lifetime属性 */
int CParticle::GetLifeTime(GLint index,GLfloat &lifetime)
{
	if(index >= 0 && index < numparticle)
	{
		lifetime=data[index].lifetime;
		return TRUE;
	}
	return FALSE;
}

/** 获取粒子的所有属性 */
int CParticle::GetAll(int index,GLint &r,GLint &g,GLint &b,         /**< 粒子的颜色 */
							GLfloat &x,GLfloat &y,GLfloat &z,		/**< 位置 */
							GLfloat &vx,GLfloat &vy,GLfloat &vz,	/**< 速度 */
							GLfloat &ax,GLfloat &ay,GLfloat &az,	/**< 加速度 */
							GLfloat &size,						    /**< 大小 */
							GLfloat &lifetime,					    /**< 生命时间 */
							GLfloat &dec					        /**< 消失速度 */
							)
{
	if (index>=0 && index<numparticle)
	{
		r=data[index].r;
		g=data[index].g;
		b=data[index].b;
		x=data[index].x;
		y=data[index].y;
		z=data[index].z;
		vx=data[index].vx;
		vy=data[index].vy;
		vz=data[index].vz;
		ax=data[index].ax;
		ay=data[index].ay;
		az=data[index].az;
		lifetime=data[index].lifetime;
		size=data[index].size;
		dec=data[index].dec;
    	return TRUE;
	}
	return FALSE;
}

/** 设置粒子的所有属性 */
int CParticle::SetAll(int index,GLint r,GLint g,GLint b,        /**< 粒子的颜色 */
							GLfloat x,GLfloat y,GLfloat z,		/**< 位置 */
							GLfloat vx,GLfloat vy,GLfloat vz,	/**< 速度 */
							GLfloat ax,GLfloat ay,GLfloat az,	/**< 加速度 */
							GLfloat size,						/**< 大小 */
							GLfloat lifetime,					/**< 生命时间 */
							GLfloat dec						    /**< 消失速度 */
							)
{
	if(index>=0 && index<numparticle)
	{
		data[index].r=r;
		data[index].g=g;
		data[index].b=b;
		data[index].x=x;
		data[index].y=y;
		data[index].z=z;
		data[index].vx=vx;
		data[index].vy=vy;
		data[index].vz=vz;
		data[index].ax=ax;
		data[index].ay=ay;
		data[index].az=az;
		data[index].lifetime=lifetime;
		data[index].size=size;
		data[index].dec=dec;
		return TRUE;
	}
	return FALSE;
}