
#pragma once

#include "PluginLibrary.h"

// exported function for main application

PLUGIN_EXPORT void PIInitPlugin(CWinApp* pApp);

// register plugin document templates
PLUGIN_EXPORT void PIRegisterDocTemplates();

PLUGIN_EXPORT void PICommand(UINT nCommandID, CCmdUI* pCmdUI);

PLUGIN_EXPORT int PIAddNewCommand(int nPluginIndex, UINT nActualID);

PLUGIN_EXPORT void PIGetCommandString(UINT nCommandID, CString& str);

PLUGIN_EXPORT void PIDraw(CDC* pDC);

PLUGIN_EXPORT BOOL PIHandleMessage(MSG* pMsg, CWnd* pSender);

struct ToolbarInfo
{
	int nPluginIndex;
	HINSTANCE hInstance;
	UINT nIDResource;
	UINT nCommandIDIndex;
	CString strText;
};

#include "PIToolBar.h"
#include "PIView.h"
#include "PIDocManager.h"
#include "PIMDIFrameWndEx.h"