
#pragma once

#include "PluginLibrary.h"

typedef void (*INITPLUGIN)(CWinApp* pApp, int nPluginIndex);
typedef void (*RELEASPLUGIN)();
typedef void (*QUERYPLUGIN)(CPluginInfo& pluginfo);
typedef void (*EXECUTEPLUGIN)(UINT nCommandID, CCmdUI* pCmdUI);
typedef void (*DRAWPLUGIN)(CDC* pDC);
typedef int (*GETDOCTEMPLATECOUNT)();
typedef CPIMultiDocTemplate* (*GETDOCTEMPLATE)(int nIndex);

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

typedef CArray<PluginCommand, PluginCommand> CommandArray;

class CPluginWrapper
{
public:
	CPluginWrapper();							// default constructor
	CPluginWrapper(CString& filename);			// filename constructor
	CPluginWrapper(CPluginWrapper& other);		// copy constructor
	virtual ~CPluginWrapper();					// destructor

	BOOL					LoadDLL(CString strFileName);				// true is load successful
	BOOL					ReleaseDLL();
	CPluginWrapper&			operator=(CPluginWrapper& other);			// does not work as regular operator=

	// Plug-in DLL wrapper functions
	void					InitPlugin(CWinApp* pApp, int nPluginIndex);
	void					ReleasePlugin();
	void					QueryPlugin();
	void					ExecutePlugin(UINT nCommandID, CCmdUI* pCmdUI);
	void					Draw(CDC* pDC);
	int						GetDocTemplateCount();
	CPIMultiDocTemplate*	GetDocTemplate(int nIndex);

	HINSTANCE				GetInstance() const;
	const CPluginInfo&		GetPluginInfo() const;

	UINT					GetFirstVirtualID() const;
	void					SetFirstVirtualID(UINT nCommandID);
	static UINT				GetCommandIDIndex();
	static void				OffsetCommandIDIndex(int nCount);

	// return command array
	const CommandArray&		GetCommandArray() const;
	// add new command
	void					AddCommand(UINT nActualID);
	// find actual command id
	UINT					FindCommand(UINT nVirtualID);
	// get count of command
	int						GetCommandCount();
	// get command string
	void					GetCommandString(UINT nVirtualID, CString& str);

private:
	// member variables
	CString					m_strFileName;		// filename of DLL
	HINSTANCE				m_hInstance;		// Instance of DLL
	CPluginInfo				m_PluginInfo;

	// first virtual ID
	UINT					m_nFirstVirtualID;
	// command index
	static UINT				m_nCommandIDIndex;

	CommandArray			m_CommandArray;

	// function pointers. Each of these function pointers has a "wrapper" function in the class
	// which allows the rest of he code not to worry about whether the pointer is NULL or not
	// DLL initialisation / de-initialisation
	INITPLUGIN				m_pFNInitPlugin;
	RELEASPLUGIN			m_pFNReleasePlugin;
	QUERYPLUGIN				m_pFNQueryPlugin;
	EXECUTEPLUGIN			m_pFNExecutePlugin;
	DRAWPLUGIN				m_pFNDrawPlugin;
	GETDOCTEMPLATECOUNT		m_pFNGetDocTemplateCount;
	GETDOCTEMPLATE			m_pFNGetDocTemplate;
};
