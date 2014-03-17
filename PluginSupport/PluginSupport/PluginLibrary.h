
#pragma once

#ifdef __cplusplus
	#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
	#define PLUGIN_EXPORT __declspec(dllexport)
#endif

#ifndef PLUGIN_NO_AUTO_LIB
	#ifdef _DEBUG
		#pragma comment(lib, "..\\..\\PluginSupport\\Debug\\PluginSupport.lib")
	#else
		#pragma comment(lib, "..\\..\\PluginSupport\\Release\\PluginSupport.lib")
	#endif
#endif

PLUGIN_EXPORT int MergeMenu(int nPluginIndex, const CMenu* pMenuAdd, BOOL bTopLevel = FALSE);

PLUGIN_EXPORT int MergeToolbar(int nPluginIndex, UINT nIDResource);

PLUGIN_EXPORT int InsertToolbar(int nPluginIndex, UINT nIDResource, CString strText);

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

class CPluginInfo
{
/*	* @name:		the plugin's name
	* @blurb:		a short text describing what the plugin does
	* @help:		the help text for the plugin (usually considerably longer than @blurb)
	* @author:		the plugin's author(s)
	* @copyright:	the plugin's copyright
	* @date:		the date the plugin was added
	* @menu_label:	the label to use for the plugin's menu entry, or #NULL if the plugin has no menu entry
	* @image_types:	the drawable types the plugin can handle
	* @type:		the type of the plugin
	* @n_params:	the number of parameters the plugin takes
	* @n_return_vals:the number of return values the plugin returns
	* @params:		the plugin's parameters
	* @return_vals:	the plugin's return values
*/
public:
	CString m_strName;
	CString m_strBlurb;
	CString m_strHelp;
	CString m_strAuthor;
	CString m_strCopyRight;
	CString m_strDate;
	CString m_strMenuLabel;
};

#ifndef PLUGIN_EXT_CLASS
	#ifdef _AFXDLL
		#define PLUGIN_EXT_CLASS	AFX_CLASS_EXPORT
	#else
		#define PLUGIN_EXT_CLASS	// not used with static builds
	#endif
#endif

#include "PIMultiDocTemplate.h"
//#include "PIDocument.h"