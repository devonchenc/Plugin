
// Demo.h : main header file for the Demo application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CDemoApp:
// See Demo.cpp for the implementation of this class
//

class CDemoApp : public CWinAppEx
{
public:
	CDemoApp();


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

public:
	// write log
	void WriteLog(CString strLog);

	// replace document manager
	void ReplaceDocManager();

	// remove plugin document template
	void RemovePluginDocTemplate();

	// switch language
	void SwitchLanguage();

// Implementation
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnLanguageEng();
	afx_msg void OnUpdateLanguageEng(CCmdUI* pCmdUI);
	afx_msg void OnLanguageChs();
	afx_msg void OnUpdateLanguageChs(CCmdUI* pCmdUI);
};

extern CDemoApp theApp;
