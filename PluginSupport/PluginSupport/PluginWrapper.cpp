
#include "stdafx.h"
#include "PluginWrapper.h"

#include "PIMultiDocTemplate.h"

UINT CPluginWrapper::m_nCommandIDIndex = PLUGIN_COMMAND_BEGIN;

CPluginWrapper::CPluginWrapper()
{
	m_hInstance = NULL;				// no DLL loaded
	m_strFileName = _T("");			// no filename

	// initialize function pointers
	m_pFNInitPlugin = NULL;
	m_pFNReleasePlugin = NULL;
	m_pFNQueryPlugin = NULL;
	m_pFNExecutePlugin = NULL;
	m_pFNDrawPlugin = NULL;
	m_pFNGetDocTemplateCount = NULL;
	m_pFNGetDocTemplate = NULL;
}

CPluginWrapper::CPluginWrapper(CPluginWrapper& other)
{
	// use the operator= functionality so its all done in one place
	*this = other;
}

CPluginWrapper::~CPluginWrapper()
{
	// release any loaded DLL
	ReleaseDLL();
}

BOOL CPluginWrapper::LoadDLL(CString strFileName)
{
	m_strFileName = strFileName;
	m_hInstance = LoadLibrary(strFileName);

	if (m_hInstance != NULL)
	{
		// get pointers to functions
		// add others here, if you add your own wrapper functions. Note also you
		// must modify the copy constructor and the operator= as well!
		m_pFNInitPlugin = (INITPLUGIN)GetProcAddress(m_hInstance, "Init");
		m_pFNReleasePlugin = (RELEASPLUGIN)GetProcAddress(m_hInstance, "Release");
		m_pFNQueryPlugin = (QUERYPLUGIN)GetProcAddress(m_hInstance, "Query");
		m_pFNExecutePlugin = (EXECUTEPLUGIN)GetProcAddress(m_hInstance, "Execute");
		m_pFNDrawPlugin = (DRAWPLUGIN)GetProcAddress(m_hInstance, "Draw");
		m_pFNGetDocTemplateCount = (GETDOCTEMPLATECOUNT)GetProcAddress(m_hInstance, "GetDocTemplateCount");
		m_pFNGetDocTemplate = (GETDOCTEMPLATE)GetProcAddress(m_hInstance, "GetDocTemplate");

		if (m_pFNInitPlugin == NULL || m_pFNReleasePlugin == NULL || m_pFNQueryPlugin == NULL)
		{
			FreeLibrary(m_hInstance);
			return FALSE;
		}
	}
	return (m_hInstance != NULL);
}

BOOL CPluginWrapper::ReleaseDLL()
{
	// release any loaded DLL
	if (m_hInstance != NULL)
	{
		FreeLibrary(m_hInstance);		// release library
		m_hInstance = NULL;
		m_strFileName = "";
		// kill any function pointers
		m_pFNInitPlugin = NULL;
		m_pFNReleasePlugin = NULL;
		m_pFNQueryPlugin = NULL;
		m_pFNExecutePlugin = NULL;
		m_pFNDrawPlugin = NULL;
		m_pFNGetDocTemplateCount = NULL;
		m_pFNGetDocTemplate = NULL;
	}
	return TRUE;
}

CPluginWrapper& CPluginWrapper::operator=(CPluginWrapper& other)
{
	// NOTE: This operator= does not work as a regular operator= in that both object will not be the same
	// after assignment! If the local object wraps a DLL already, then it is freed. The other object gets
	// copied across and removed from the other object, so that 2 CDLLWrapper object cannot wrap the same
	// object. This makes them unique
	ReleaseDLL();						// free our DLL if we have one
	m_strFileName = other.m_strFileName;
	other.m_strFileName = "";
	m_hInstance = other.m_hInstance;
	other.m_hInstance = NULL;
	m_pFNInitPlugin = other.m_pFNInitPlugin;
	other.m_pFNInitPlugin = NULL;
	m_pFNReleasePlugin = other.m_pFNReleasePlugin;
	other.m_pFNReleasePlugin  = NULL;
	m_pFNQueryPlugin = other.m_pFNQueryPlugin;
	other.m_pFNQueryPlugin = NULL;
	m_pFNExecutePlugin = other.m_pFNExecutePlugin;
	other.m_pFNExecutePlugin = NULL;
	m_pFNDrawPlugin = other.m_pFNDrawPlugin;
	other.m_pFNDrawPlugin = NULL;
	m_pFNGetDocTemplateCount = other.m_pFNGetDocTemplateCount;
	other.m_pFNGetDocTemplateCount = NULL;
	m_pFNGetDocTemplate = other.m_pFNGetDocTemplate;
	other.m_pFNGetDocTemplate = NULL;

	// allow cascading =
	// a = b = c; etc
	return *this;
}

void CPluginWrapper::InitPlugin(CWinApp* pApp, int nPluginIndex)
{
	// map virtual plugin command id
	SetFirstVirtualID(CPluginWrapper::GetCommandIDIndex());

	if (m_pFNInitPlugin != NULL)
	{
		m_pFNInitPlugin(pApp, nPluginIndex);
	}
}

void CPluginWrapper::ReleasePlugin()
{
	if (m_pFNReleasePlugin)
	{
		m_pFNReleasePlugin();
	}
}

void CPluginWrapper::QueryPlugin()
{
	if (m_pFNQueryPlugin)
	{
		m_pFNQueryPlugin(m_PluginInfo);
	}
}

void CPluginWrapper::ExecutePlugin(UINT nCommandID, CCmdUI* pCmdUI)
{
	UINT nActualID = FindCommand(nCommandID);
	ASSERT(nActualID != 0);

	if (m_pFNExecutePlugin)
	{
		m_pFNExecutePlugin(nActualID, pCmdUI);
	}
}

void CPluginWrapper::Draw(CDC* pDC)
{
	if (m_pFNDrawPlugin)
	{
		m_pFNDrawPlugin(pDC);
	}
}

int CPluginWrapper::GetDocTemplateCount()
{
	if (m_pFNGetDocTemplateCount)
	{
		return m_pFNGetDocTemplateCount();
	}
	else
	{
		return 0;
	}
}

CPIMultiDocTemplate* CPluginWrapper::GetDocTemplate(int nIndex)
{
	if (m_pFNGetDocTemplate)
	{
		ASSERT(nIndex >= 0);
		ASSERT(nIndex < GetDocTemplateCount());
		return m_pFNGetDocTemplate(nIndex);
	}
	else
	{
		return NULL;
	}
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
void CPluginWrapper::AddCommand(UINT nActualID)
{
	PluginCommand command;
	command.nActualID = nActualID;
	command.nVirtualID = GetCommandIDIndex();
	m_CommandArray.Add(command);

	m_nCommandIDIndex++;
}

// find actual command id
UINT CPluginWrapper::FindCommand(UINT nVirtualID)
{
	for (int i=0; i<m_CommandArray.GetCount(); i++)
	{
		PluginCommand command = m_CommandArray.GetAt(i);
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