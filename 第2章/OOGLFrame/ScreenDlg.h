//========================================================
/**
*  @file  ScreenDlg.h
*
*  项目描述： 使用OO的OpenGL程序框架演示
*  文件描述:  模式选择对话框类    
*  适用平台： Windows98/2000/NT/XP
*  
*  作者：     BrightXu
*  电子邮件:  huoxini@hotmail.com
*  创建日期： 2006-10-13	
*  修改日期： 2006-11-07
*
*/     
//========================================================
#ifndef	__SCREENDLG_H__
#define	__SCREENDLG_H__

#include "GLWindow.h"											/**< 包含GLWindow.h头文件 */

/** 定义模式选择对话框类 */
class ScreenDlg
{
public:
	ScreenDlg(void);   /**< 构造函数 */
	~ScreenDlg(void);  /**< 析构函数 */

	void SetupWindow(GLWindow *pWindow); /**< 配置窗口，并弹出对话框 */
    
	///对话框消息处理回调函数
	static LRESULT CALLBACK ScreenDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);

private:
	static GLWindow *m_pWindow; /**< 全局窗口类 */
};
#endif //__SCREENDLG_H__