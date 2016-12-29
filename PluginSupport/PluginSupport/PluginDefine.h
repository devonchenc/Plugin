#pragma once

#ifndef PLUGIN_EXT_CLASS
	#ifdef PLUGIN_NO_AUTO_LIB
		#define PLUGIN_EXT_CLASS	AFX_CLASS_EXPORT
		#define PLUGIN_EXT_TEMPLATE AFX_CLASS_EXPORT
	#else
		#define PLUGIN_EXT_CLASS	AFX_CLASS_IMPORT
		#define PLUGIN_EXT_TEMPLATE
	#endif
#endif

#ifdef __cplusplus
	#define PLUGIN_EXPORT extern "C" __declspec(dllexport)
#else
	#define PLUGIN_EXPORT __declspec(dllexport)
#endif

#ifndef PLUGIN_VERSION
#define PLUGIN_VERSION	0.1
#endif

#ifndef SAFE_DELETE
#define SAFE_DELETE(p)			do{delete(p);(p)=NULL;} while(0)
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p)	do{delete[](p);(p)=NULL;} while(0)
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
#define WM_CREATE_WIDGET			WM_USER + 112
#define WMU_FILE_NEW				WM_USER + 113
#define WMU_RIBBONBAR_EVENT			WM_USER + 114

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
	* @version:		the plugin's version
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
	CString m_strVersion;
	CString m_strCopyRight;
	CString m_strDate;
	CString m_strMenuLabel;
};

#define PI_PROGRESS_THREAD_DLG		0
#define PI_PROGRESS_NATIVE_DLG		1
#define PI_PROGRESS_BAR				2


// Macro definition
#define DECLARE_PLUGIN(class_name) \
public: \
	static class_name* Instance(){ static class_name _instance; return &_instance; } \
	virtual double GetVersion(){ return PLUGIN_VERSION; } \
	virtual void Execute(UINT nCommandID, CCmdUI* pCmdUI) \
	{ \
		AFX_MANAGE_STATE(AfxGetStaticModuleState()); \
		CWinApp* pApp = AfxGetApp(); \
		if (pCmdUI == NULL) \
		{ \
			pApp->OnCmdMsg(nCommandID, CN_COMMAND, NULL, NULL); \
		} \
		else \
		{ \
			pApp->OnCmdMsg(nCommandID, CN_UPDATE_COMMAND_UI, pCmdUI, NULL); \
		} \
	}

#define IMPLEMENT_PLUGIN(class_name) \
	PLUGIN_EXPORT class_name* GetInstance(){ return class_name::Instance(); }

// 声明具有动态创建的基类
#define DECLARE_DYNCRT_BASE(CBase) \
public: \
	typedef CBase* (*ClassGen)(); /* 声明函数指针*/ \
	static CBase* Create(const char* class_name) /* 工厂函数 */ \
	{ \
		CString str(class_name); \
		ClassGen class_gen; \
		if (m_ClassSet.Lookup(str, class_gen)) \
		{ \
			return class_gen(); \
		} \
		return NULL; \
	} \
public: \
	static void Register(const char* class_name, ClassGen class_gen) /* 注册函数 */ \
	{ \
		CString str(class_name); \
		m_ClassSet.SetAt(str, class_gen); \
	} \
	static void Unregister(const char* class_name) /* 反注册函数 */ \
	{ \
		CString str(class_name); \
		m_ClassSet.RemoveKey(str); \
	} \
static CMap<CString, LPCTSTR, ClassGen, ClassGen> m_ClassSet; /* 存储子类信息 */

// 用于实现基类
#define IMPLEMENT_DYNCRT_BASE(CBase) \
	CMap<CString, LPCTSTR, CBase::ClassGen, CBase::ClassGen> CBase::m_ClassSet;

// 用于声明一个能够被动态创建的类(用一个全局对象进行注册)
#define DECLARE_DYNCRT_CLASS(CDerived, CBase) \
public: \
	struct CDerived##Register /* 辅助类，用于注册 */ \
	{ \
		CDerived##Register() \
		{ \
			static bool bRegistered = false; /* 注册子类，虽然map能保证唯一，但仍只注册一次 */ \
			if (!bRegistered) \
			{ \
				CBase::Register(#CDerived, CDerived::Create); /* 注册子类信息 */ \
				bRegistered = true; \
			} \
		} \
		~CDerived##Register() \
		{ \
			CBase::Unregister(#CDerived); \
		} \
	}; \
	static CBase* Create() /* 工厂函数 */ \
	{ \
		return new CDerived; \
	} \
	static struct CDerived##Register m_t##CDerived##Register;

// 用于实现一个能被动态创建的类
#define IMPLEMENT_DYNCRT_CLASS(CDerived) \
	static CDerived::CDerived##Register m_t##CDerived##Register;