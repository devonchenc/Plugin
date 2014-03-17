// PluginSupport.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "PluginSupport.h"
#include "PluginWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CPluginSupportApp

BEGIN_MESSAGE_MAP(CPluginSupportApp, CWinApp)
END_MESSAGE_MAP()


// CPluginSupportApp 构造

CPluginSupportApp::CPluginSupportApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CPluginSupportApp 对象

CPluginSupportApp theApp;


// CPluginSupportApp 初始化

BOOL CPluginSupportApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

int CPluginSupportApp::ExitInstance()
{
	for (int i=0; i<m_PluginArray.GetSize(); i++)
	{
		CPluginWrapper* pPluginWrapper = m_PluginArray.GetAt(i);
		pPluginWrapper->ReleasePlugin();
		delete pPluginWrapper;
	}
	m_PluginArray.RemoveAll();

	m_pMainApp = NULL;

	return CWinApp::ExitInstance();
}

// record main app
void CPluginSupportApp::SetMainApp(CWinApp* pApp)
{
	m_pMainApp = pApp;
}

// return main app
CWinApp* CPluginSupportApp::GetMainApp() const
{
	return m_pMainApp;
}

// load plugin
void CPluginSupportApp::LoadPlugin(CWinApp* pApp)
{
	CString strPath = GetApplicationPath();
	strPath += _T("*.dll");

	CFileFind find;
	BOOL bIsFind = find.FindFile(strPath);
	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			continue;
		else
		{
			CString strFileName = find.GetFileName();

			CPluginWrapper* pPluginWrapper = new CPluginWrapper();
			BOOL bReslut = pPluginWrapper->LoadDLL(strFileName);
			if (bReslut)
			{
				m_PluginArray.Add(pPluginWrapper);
			}
			else
			{
				delete pPluginWrapper;
			}
		}
	}

	// initialization plugin
	for (int i=0; i<m_PluginArray.GetSize(); i++)
	{
		m_PluginArray.GetAt(i)->InitPlugin(pApp, i);
		m_PluginArray.GetAt(i)->QueryPlugin();
	}
}

// return plugin array
const PluginArray& CPluginSupportApp::GetPluginArray() const
{
	return m_PluginArray;
}

// return plugin message map
MessageMap& CPluginSupportApp::GetPluginMessageMap()
{
	return m_MessageMap;
}

CString CPluginSupportApp::GetApplicationPath()
{
	TCHAR filename[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR path[_MAX_PATH];

	GetModuleFileName(AfxGetInstanceHandle(), filename, MAX_PATH);
	_wsplitpath_s(filename, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, 0, NULL, 0);

	CString strLocation(drive);
	strLocation += path;

	return strLocation;
}