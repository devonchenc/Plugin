
#pragma once

#include "PluginLibrary.h"

typedef CPlugin* (*GETINSTANCE)();

typedef CArray<CPluginCommand, CPluginCommand> CommandArray;

class CPluginWrapper
{
public:
	CPluginWrapper();							// default constructor
	virtual ~CPluginWrapper();					// destructor

private:
	CPluginWrapper(CPluginWrapper& other){}		// copy constructor
	CPluginWrapper& operator=(CPluginWrapper& other){}

public:
	// return true is load successful
	BOOL LoadDLL(CString strFileName);

	// Plug-in DLL wrapper functions
	void InitPlugin(CWinApp* pApp, int nPluginIndex);
	void ReleasePlugin();
	void QueryPlugin();
	void ExecutePlugin(UINT nCommandID, CCmdUI* pCmdUI);
	void Draw(CDC* pDC);
	int GetDocTemplateCount();
	CPIMultiDocTemplate* GetDocTemplate(int nIndex);

	HINSTANCE GetInstance() const;
	const CPluginInfo& GetPluginInfo() const;

	UINT GetFirstVirtualID() const;
	void SetFirstVirtualID(UINT nCommandID);
	static UINT GetCommandIDIndex();
	static void OffsetCommandIDIndex(int nCount);

	// return command array
	const CommandArray& GetCommandArray() const;
	// add new command
	int AddCommand(UINT nActualID);
	// find actual command id
	UINT FindCommand(UINT nVirtualID);
	// get count of command
	int GetCommandCount();
	// get command string
	void GetCommandString(UINT nVirtualID, CString& str);

	// member variables
private:
	// filename of DLL
	CString m_strFileName;

	// Instance of DLL
	HINSTANCE m_hInstance;

	// index of all plugin
	int m_nPluginIndex;

	CPluginInfo m_PluginInfo;

	// first virtual ID
	UINT m_nFirstVirtualID;
	// command index
	static UINT m_nCommandIDIndex;

	CommandArray m_CommandArray;

	// function pointers. Each of these function pointers has a "wrapper" function in the class
	// which allows the rest of he code not to worry about whether the pointer is NULL or not
	// DLL initialisation / de-initialisation
	GETINSTANCE m_pfnGetInstance;

	CPlugin* m_pPlugin;
};
