// PluginSupport.h : PluginSupport DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CPluginSupportApp
// 有关此类实现的信息，请参阅 PluginSupport.cpp
//

class CPluginWrapper;

typedef CArray<CPluginWrapper*, CPluginWrapper*> PluginArray;
typedef LRESULT (__cdecl* MESSAGEPROC)(MSG* pMsg);
typedef CMap<UINT, UINT, MESSAGEPROC, MESSAGEPROC> MessageMap;

class CPluginSupportApp : public CWinApp
{
public:
	CPluginSupportApp();

public:
	// record main app
	void SetMainApp(CWinApp* pApp);

	// return main app
	CWinApp* GetMainApp() const;

	// load plugin
	void LoadPlugin(CWinApp* pApp);

	// return plugin array
	const PluginArray& GetPluginArray() const;

	// return plugin message map
	MessageMap& GetPluginMessageMap();

private:
	CString GetApplicationPath();

private:
	// main app point
	CWinApp* m_pMainApp;

	// plugin array
	PluginArray m_PluginArray;

	// plugin message map
	MessageMap m_MessageMap;

// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	DECLARE_MESSAGE_MAP()
};
