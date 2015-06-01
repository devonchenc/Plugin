#pragma once

#ifndef PLUGIN_EXT_CLASS
	#ifdef _AFXDLL
		#define PLUGIN_EXT_CLASS	AFX_CLASS_EXPORT
	#else
		#define PLUGIN_EXT_CLASS	// not used with static builds
	#endif
#endif

// define message
#define WM_RESERVED					WM_USER + 100
#define WM_MENU_EVENT				WM_USER + 101
#define WM_TOOLBAR_EVENT			WM_USER + 102
#define WM_UI_ENABLE_EVENT			WM_USER + 103
#define WM_UI_CHECK_EVENT			WM_USER + 104
#define WM_GET_ACTIVE_DOC			WM_USER + 105
#define WM_GET_ACTIVE_VIEW			WM_USER + 106
#define WM_PREPARE_DC_EVENT			WM_USER + 107
#define WM_PROGRESS_INIT			WM_USER + 108
#define WM_PROGRESS_PERCENT			WM_USER + 109
#define WM_PROGRESS_DONE			WM_USER + 110
#define WM_CREATE_DOCKABLE_PANE		WM_USER + 111

#ifndef PLUGIN_COMMAND
	#define PLUGIN_COMMAND
	// plugin command begin & end
	#define PLUGIN_COMMAND_BEGIN	50000
	#define PLUGIN_COMMAND_END		57599
	struct CPluginCommand
	{
		int nPluginIndex;
		UINT nActualID;
		UINT nVirtualID;
	};
#endif

#ifndef PLUGIN_WINDOW
	#define PLUGIN_WINDOW
	#define PLUGIN_WINDOW_ID_BEGIN	100
	struct CPluginWindow
	{
		HINSTANCE hInstance;
		CWnd* pWnd;
		CRect rect;
		BOOL bShow;
		LPCTSTR lpszCaption;
	};
#endif

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

#define PI_PROGRESS_DLG		0
#define PI_PROGRESS_BAR		1