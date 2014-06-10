
#include "stdafx.h"
#include "PluginWrapper.h"
#include "PIMultiDocTemplate.h"

UINT CPluginWrapper::m_nCommandIDIndex = PLUGIN_COMMAND_BEGIN;

CPluginWrapper::CPluginWrapper()
	: m_strFileName(_T(""))
	, m_hInstance(NULL)
	, m_pfnGetInstance(NULL)
	, m_pPlugin(NULL)
{

}

CPluginWrapper::~CPluginWrapper()
{
	// release any loaded DLL
	if (m_hInstance != NULL)
	{
		FreeLibrary(m_hInstance);	// release library
		m_hInstance = NULL;
		m_strFileName = "";
		// kill any function pointers
		m_pfnGetInstance = NULL;
	}
}

BOOL CPluginWrapper::LoadDLL(CString strFileName)
{
	m_strFileName = strFileName;
	m_hInstance = LoadLibrary(strFileName);

	if (m_hInstance != NULL)
	{
		m_pfnGetInstance = (GETINSTANCE)GetProcAddress(m_hInstance, "GetInstance");
		if (m_pfnGetInstance == NULL)
		{
			FreeLibrary(m_hInstance);
			return FALSE;
		}

		m_pPlugin = m_pfnGetInstance();
		if (m_pPlugin == NULL)
		{
			FreeLibrary(m_hInstance);
			return FALSE;
		}
	}
	return (m_hInstance != NULL);
}

void CPluginWrapper::InitPlugin(CWinApp* pApp, int nPluginIndex)
{
	m_nPluginIndex = nPluginIndex;

	// map virtual plugin command id
	SetFirstVirtualID(CPluginWrapper::GetCommandIDIndex());

	m_pPlugin->SetPluginIndex(nPluginIndex);
	m_pPlugin->Init();
}

void CPluginWrapper::ReleasePlugin()
{
	m_pPlugin->Release();
}

void CPluginWrapper::QueryPlugin()
{
	m_pPlugin->Query(m_PluginInfo);
}

void CPluginWrapper::ExecutePlugin(UINT nCommandID, CCmdUI* pCmdUI)
{
	UINT nActualID = FindCommand(nCommandID);
	ASSERT(nActualID != 0);

	m_pPlugin->Execute(nActualID, pCmdUI);
}

void CPluginWrapper::Draw(CDC* pDC)
{
	m_pPlugin->Draw(pDC);
}

int CPluginWrapper::GetDocTemplateCount()
{
	return m_pPlugin->GetDocTemplateCount();
}

CPIMultiDocTemplate* CPluginWrapper::GetDocTemplate(int nIndex)
{
	ASSERT(nIndex >= 0);
	ASSERT(nIndex < GetDocTemplateCount());
	return m_pPlugin->GetDocTemplate(nIndex);
}

HINSTANCE CPluginWrapper::GetInstance() const
{
	return m_hInstance;
}

const CPluginInfo& CPluginWrapper::GetPluginInfo() const
{
	return m_PluginInfo;
}

UINT CPluginWrapper::GetFirstVirtualID() const
{
	return m_nFirstVirtualID;
}

void CPluginWrapper::SetFirstVirtualID(UINT nCommandID)
{
	m_nFirstVirtualID = nCommandID;
}

UINT CPluginWrapper::GetCommandIDIndex()
{
	return m_nCommandIDIndex;
}

void CPluginWrapper::OffsetCommandIDIndex(int nCount)
{
	m_nCommandIDIndex += nCount;
}

// return command array
const CommandArray& CPluginWrapper::GetCommandArray() const
{
	return m_CommandArray;
}

// add new command
int CPluginWrapper::AddCommand(UINT nActualID)
{
	for (int i=0; i<m_CommandArray.GetCount(); i++)
	{
		CPluginCommand command = m_CommandArray.GetAt(i);
		if (m_nPluginIndex == command.nPluginIndex && nActualID == command.nActualID)
		{
			// already exist
			return command.nVirtualID;
		}
	}

	CPluginCommand command;
	command.nPluginIndex = m_nPluginIndex;
	command.nActualID = nActualID;
	command.nVirtualID = GetCommandIDIndex();
	m_CommandArray.Add(command);

	m_nCommandIDIndex++;

	return command.nVirtualID;
}

// find actual command id
UINT CPluginWrapper::FindCommand(UINT nVirtualID)
{
	for (int i=0; i<m_CommandArray.GetCount(); i++)
	{
		CPluginCommand command = m_CommandArray.GetAt(i);
		if (command.nVirtualID == nVirtualID)
		{
			return command.nActualID;
			break;
		}
	}

	return 0;
}

// get count of command
int CPluginWrapper::GetCommandCount()
{
	return m_CommandArray.GetCount();
}

// get toolbar command string
void CPluginWrapper::GetCommandString(UINT nVirtualID, CString& str)
{
	UINT nActualID = FindCommand(nVirtualID);
	ASSERT(nActualID != 0);

	str.LoadString(m_hInstance, nActualID);

	// delete '\n'
	int nIndex = str.Find(_T('\n'));
	if (nIndex != -1)
	{
		str = str.Left(nIndex);
	}
}