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


#ifndef PLUGIN_COMMAND
	#define PLUGIN_COMMAND
	// plugin command begin & end
	#define PLUGIN_COMMAND_BEGIN	50000
	#define PLUGIN_COMMAND_END		57599
	struct PluginCommand
	{
		UINT nActualID;
		UINT nVirtualID;
	};
#endif