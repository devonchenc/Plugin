
#include "stdafx.h"
#include "PluginSupport.h"
#include "PluginLibrary.h"
#include "PluginImpl.h"
#include "PluginWrapper.h"

PLUGIN_EXPORT void PIEnableUI(CCmdUI* pCmdUI, BOOL bEnable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_UI_ENABLE_EVENT, (WPARAM)pCmdUI, (LPARAM)bEnable);
	}
}

PLUGIN_EXPORT void PICheckUI(CCmdUI* pCmdUI, BOOL bCheck)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_UI_CHECK_EVENT, (WPARAM)pCmdUI, (LPARAM)bCheck);
	}
}

PLUGIN_EXPORT void PIMessageBox(CString str, UINT uType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();
	CWnd* pMainWnd = pMainApp->m_pMainWnd;
	if (pMainWnd)
	{
		pMainWnd->MessageBox(str, pMainApp->m_pszAppName, uType);
	}
}

PLUGIN_EXPORT void PILog(CString strLog)
{
	CString strAppName = AfxGetApp()->m_pszAppName;
	strLog = strAppName + _T("> ") + strLog;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;

	COPYDATASTRUCT copyData = {0};
	copyData.lpData = strLog.GetBuffer();
	copyData.cbData = strLog.GetLength();
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_COPYDATA, (WPARAM)NULL, (LPARAM)&copyData);
	}
}

PLUGIN_EXPORT void PIPrepareDC(CView* pView, CDC* pDC)
{
	pView->SendMessage(WM_PREPARE_DC_EVENT, (WPARAM)NULL, (LPARAM)pDC);
}

PLUGIN_EXPORT void PIRegisterMessage(int nPluginIndex, UINT nMessage, MESSAGEPROC pFN)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	MessageMap& messageMap = pApp->GetPluginMessageMap();
	messageMap.SetAt(nMessage, pFN);
}

PLUGIN_EXPORT void PIUnregisterMessage(int nPluginIndex, UINT nMessage)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	MessageMap& messageMap = pApp->GetPluginMessageMap();
	BOOL bResult = messageMap.RemoveKey(nMessage);
	if (!bResult)
	{
		TRACE(_T("Plugin: messageMap RemoveKey %d was not found!\n"), nMessage);
	}
}

PLUGIN_EXPORT CDocument* PIGetActiveDocument()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;

	CDocument* pActiveDoc = NULL;
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_GET_ACTIVE_DOC, (WPARAM)&pActiveDoc, (LPARAM)NULL);
	}
	
	return pActiveDoc;
}

PLUGIN_EXPORT CView* PIGetActiveView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;

	CView* pActiveView = NULL;
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_GET_ACTIVE_VIEW, (WPARAM)&pActiveView, (LPARAM)NULL);
	}

	return pActiveView;
}

// Progress Dialog
PLUGIN_EXPORT void PIProgressInit(LPCTSTR lpszText)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;
	if (pMainWnd == NULL)
		return;

	CDialog* pDialog = (CDialog*)pMainWnd->SendMessage(WM_PROGRESS_INIT, (WPARAM)NULL, (LPARAM)lpszText);
	if (pDialog == NULL)
		return;

	int nCount = 0;
	while (pDialog->GetSafeHwnd() == NULL && nCount < 10)
	{
		nCount++;
		Sleep(100);
	}

	// set progress dialog title
	if (pDialog->GetSafeHwnd())
	{
		if (lpszText)
		{
			pDialog->SetWindowText(lpszText);
		}
		else
		{
			// remove title bar
			pDialog->ModifyStyle(WS_CAPTION, 0);

			CRect rect;
			pDialog->GetWindowRect(rect);
			pDialog->SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height() - 32, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
		}
	}
}

PLUGIN_EXPORT void PIProgressPercent(int nPercent)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_PROGRESS_PERCENT, (WPARAM)NULL, (LPARAM)nPercent);
	}
}

PLUGIN_EXPORT void PIProgressDone()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWnd* pMainWnd = pApp->GetMainApp()->m_pMainWnd;
	if (pMainWnd)
	{
		pMainWnd->SendMessage(WM_PROGRESS_DONE, (WPARAM)NULL, (LPARAM)NULL);
	}
}

PLUGIN_EXPORT LANGID PIGetThreadUILanguage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();
	int nLanguage = pMainApp->GetProfileInt(_T("Settings"), _T("Language"), 0);

	LANGID id;
	if (nLanguage == 0)
	{
		id = MAKELCID(MAKELANGID(LANG_ENGLISH, SUBLANG_ENGLISH_US), SORT_DEFAULT);
	}
	else
	{
		id = MAKELCID(MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED), SORT_DEFAULT);
	}
	return id;
}