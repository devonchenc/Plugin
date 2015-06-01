
#pragma once

#include "PluginDefine.h"

#ifndef PLUGIN_NO_AUTO_LIB
	#ifdef _DEBUG
		#pragma comment(lib, "..\\..\\PluginSupport\\Debug\\PluginSupport.lib")
		#pragma message("Automatically link with PluginSupport Debug")
	#else
		#pragma comment(lib, "..\\..\\PluginSupport\\Release\\PluginSupport.lib")
		#pragma message("Automatically link with PluginSupport Release")
	#endif
#endif

#ifdef __cplusplus
	#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
	#define PLUGIN_EXPORT __declspec(dllexport)
#endif

PLUGIN_EXPORT void PIEnableUI(CCmdUI* pCmdUI, BOOL bEnable);

PLUGIN_EXPORT void PICheckUI(CCmdUI* pCmdUI, BOOL bCheck);

PLUGIN_EXPORT void PIMessageBox(CString str, UINT uType = MB_OK | MB_ICONINFORMATION);

PLUGIN_EXPORT void PILog(CString str);

PLUGIN_EXPORT void PIPrepareDC(CView* pView, CDC* pDC);

// register message handler
typedef LRESULT (__cdecl* MESSAGEPROC)(MSG* pMsg);
PLUGIN_EXPORT void PIRegisterMessage(int nPluginIndex, UINT nMessage, MESSAGEPROC pFN);

PLUGIN_EXPORT void PIUnregisterMessage(int nPluginIndex, UINT nMessage);

PLUGIN_EXPORT CDocument* PIGetActiveDocument();

PLUGIN_EXPORT CView* PIGetActiveView();

// Progress Dialog&StatusBar
#define PI_PROGRESS_DLG		0
#define PI_PROGRESS_BAR		1
PLUGIN_EXPORT void PIProgressInit(BOOL bDlgOrBar = PI_PROGRESS_DLG, LPCTSTR lpszText = NULL);

PLUGIN_EXPORT LRESULT PIProgressPercent(int nPercent);

PLUGIN_EXPORT void PIProgressDone();

PLUGIN_EXPORT LANGID PIGetThreadUILanguage();

PLUGIN_EXPORT void PIDockablePane(CPluginWindow* pPluginWindow);

#ifndef PLUGIN_EXT_CLASS
	#ifdef _AFXDLL
		#define PLUGIN_EXT_CLASS	AFX_CLASS_EXPORT
	#else
		#define PLUGIN_EXT_CLASS	// not used with static builds
	#endif
#endif

// Macro definition
#define DECLARE_PLUGIN(class_name) \
	static class_name* Instance(){ static class_name _instance; return &_instance; }

#define IMPLEMENT_PLUGIN(class_name) \
	PLUGIN_EXPORT class_name* GetInstance(){ return class_name::Instance(); }

#include "PluginClass.h"
#include <atlimage.h>
#include <afxcontrolbars.h>
#include "PIDocument.h"
#include "PIMDIChildWndEx.h"