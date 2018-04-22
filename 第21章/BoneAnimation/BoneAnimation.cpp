//=========================================================================
/**
*  @file      BoneAnimation.cpp
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
#include "stdafx.h"
#include "BoneAnimation.h"						    

/** 控制场景旋转 */
extern float xRotation ;
extern float yRotation ;

/** 创建一个程序的实例 */
GLApplication * GLApplication::Create(const char * class_name)
{
	Test * test = new Test(class_name);
	return reinterpret_cast<GLApplication *>(test);
}

/** 构造函数 */
Test::Test(const char * class_name) : GLApplication(class_name)
{
  /// 初始化用户自定义的程序变量

}


/** 初始化OpenGL */
bool Test::Init()									
{
/** 用户自定义的初始化过程 */
   glClearColor(0.5f, 0.5f, 1.0f, 0.0f);
   glShadeModel(GL_SMOOTH);
   glEnable(GL_DEPTH_TEST);		
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);	

   /** 构造骨骼 */
   bones[0].SetBone(-1, 4.5f);
   bones[1].SetBone(0, 4.5f);

   /** 设置模型每个顶点数据 */
   modelPoints[0].SetVertex(CVector4(-1.0f, 0.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1);
   modelPoints[1].SetVertex(CVector4(1.0f, 0.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1);
   modelPoints[2].SetVertex(CVector4(1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2);
   modelPoints[3].SetVertex(CVector4(-1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 2);

   modelPoints[4].SetVertex(CVector4(-1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
   modelPoints[5].SetVertex(CVector4(1.0f, 3.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.50f, 0.50f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
   modelPoints[6].SetVertex(CVector4(1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);
   modelPoints[7].SetVertex(CVector4(-1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 2);

   modelPoints[8].SetVertex(CVector4(-1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 2);
   modelPoints[9].SetVertex(CVector4(1.0f, 6.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                            0, 1, 0, 0, 0.35f, 0.65f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 2);
   modelPoints[10].SetVertex(CVector4(1.0f, 9.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             1, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1);
   modelPoints[11].SetVertex(CVector4(-1.0f, 9.0f, 0.0f), CVector4(0.0f, 0.0f, 1.0),
                             1, 0, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0, 0.0f, 0.0f, 1.0f, 1);

  
	ResizeDraw(true);

	return true;                                            /**< 成功返回 */
}

/** 用户自定义的卸载函数 */
void Test::Uninit()									
{
	
}

/** 程序更新函数 */
void Test::Update(DWORD milliseconds)						
{
	if (m_Keys.IsPressed(VK_ESCAPE) == true)					/**< 按ESC退出 */
	{
		TerminateApplication();									
	}

    
}

/** 骨骼更新 */
void Test::UpdateBones()
{
	/** 用于保存旋转和平移矩阵 */ 
	CMatrix4x4 transpose, rotTemp1, rotTemp2, tempRelative;

   /** 循环更新骨骼 */
   for(int i = 0; i < MAX_BONES; i++)
      {
         ///检查是否是根骨骼
         if(bones[i].parent == -1)
            {
               /** 设置变换矩阵 */
               bones[i].relative.Translate(trans.x, trans.y, trans.z);
               bones[i].absolute = bones[i].relative;

               /** 赋值，为了显示骨骼 */
               displayBones[i] = bones[i].relative;
            }
         else
            {
               bones[i].relative.Clear();
               
               /** 移动该骨骼到父结点骨骼 */
               bones[i].relative.Translate(0, bones[bones[i].parent].length * i, 0);

               /** 根据鼠标旋转 */
               rotTemp1.Rotate(xRotation, 0, 1, 0);
               rotTemp2.Rotate(yRotation, 0, 0, 1);

               /** 保存相对变换矩阵，并反转 */
               tempRelative = bones[i].relative;
               tempRelative.inverseTranslate();

               /** 保存变换，为了显示骨骼 */
               displayBones[i] = bones[bones[i].parent].absolute * bones[i].relative *
                                 (rotTemp1 * rotTemp2);

               /** 计算相对变换矩阵 */
               bones[i].relative = bones[i].relative * (rotTemp1 * rotTemp2) * tempRelative;
               
               /** 计算绝对变换矩阵 */
               bones[i].absolute = bones[bones[i].parent].absolute * bones[i].relative;
            }
      }
}

/** 绘制函数 */
void Test::Draw()											
{
/** 用户自定义的绘制过程 */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );			
	glLoadIdentity();
	
	glTranslatef(0.0f, -4.0f, -15.0f);
	
	/** 更新骨骼列表 */
	UpdateBones();
	
	CMatrix4x4 mat;

   /** 渲染网格中的顶点 */
   for(int j = 0; j < MAX_MESHES; j++)
   {
      glBegin(GL_QUADS);

      for(int i = 0; i < MAX_MESH_POINTS; i++)
         {
            ///获得顶点指针
            CBoneVertex *thisVertex = &modelPoints[i + j * MAX_MESH_POINTS];

            ///根据权值计算顶点
            CVector4 pos = CVector4(0.0, 0.0, 0.0, 0.0);
            CVector4 normal = CVector4(0.0, 0.0, 0.0);

			
            for(int i2 = 0; i2 < thisVertex->numBones; i2++)
               {
                  
                  mat = bones[thisVertex->boneIndex[i2]].absolute;

                  ///加权骨骼对顶点位置和法线的影响   
                  pos += (mat.VectorMatrixMultiply(thisVertex->pos) * thisVertex->weights[i2]);
                  normal += (mat.VectorMatrixMultiply3x3(thisVertex->normal) *
                             thisVertex->weights[i2]);
               }

            ///渲染该顶点
            glColor4f(thisVertex->r, thisVertex->g, thisVertex->b, thisVertex->a);
            glNormal3f(normal.x, normal.y, normal.z);
            glVertex3f(pos.x, pos.y, pos.z);
         }

      glEnd();
   }

   //绘制骨骼
   for(int i = 0; i < MAX_BONES; i++)
      {
         glPushMatrix();

            glMultMatrixf(displayBones[i].matrix);
      
            glColor3f(1.0f, 1.0f, 1.0f);

            glBegin(GL_LINES);

               ///绘制线段
               glVertex3f(-0.4f, 0.0f, -0.4f);
               glVertex3f(0.4f, 0.0f, -0.4f);
               glVertex3f(0.4f, 0.0f, -0.4f);
               glVertex3f(0.4f, 0.0f, 0.4f);
               glVertex3f(0.4f, 0.0f, 0.4f);
               glVertex3f(-0.4f, 0.0f, 0.4f);
               glVertex3f(-0.4f, 0.0f, 0.4f);
               glVertex3f(-0.4f, 0.0f, -0.4f);

               
               glVertex3f(-0.4f, 0.0f, -0.4f);
               glVertex3f(0.0f, bones[i].length, 0.0f);
               glVertex3f(0.4f, 0.0f, -0.4f);
               glVertex3f(0.0f, bones[i].length, 0.0f);
               glVertex3f(0.4f, 0.0f, 0.4f);
               glVertex3f(0.0f, bones[i].length, 0.0f);
               glVertex3f(-0.4f, 0.0f, 0.4f);
               glVertex3f(0.0f, bones[i].length, 0.0f);

            glEnd();

         glPopMatrix();
      }

	    
	/** 强制执行所有的OpenGL命令 */
	glFlush();	                 
}
