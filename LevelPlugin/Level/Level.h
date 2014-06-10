// Level.h : Level DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "LevelDlg.h"

// CLevelApp
// 有关此类实现的信息，请参阅 Level.cpp
//

class CLevelApp : public CWinApp
{
public:
	CLevelApp();

private:
	CLevelDlg m_LevelDlg;

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnImageLevel();
	afx_msg void OnUpdateImageLevel(CCmdUI* pCmdUI);
};
