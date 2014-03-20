
#include "stdafx.h"
#include "PluginSupport.h"
#include "PluginLibrary.h"
#include "PluginImpl.h"
#include "PluginWrapper.h"

PLUGIN_EXPORT void PIEnableUI(CCmdUI* pCmdUI, BOOL bEnable)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();
	pMainApp->m_pMainWnd->SendMessage(WM_UI_ENABLE_EVENT, (WPARAM)pCmdUI, (LPARAM)bEnable);
}

PLUGIN_EXPORT void PICheckUI(CCmdUI* pCmdUI, BOOL bCheck)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();
	pMainApp->m_pMainWnd->SendMessage(WM_UI_CHECK_EVENT, (WPARAM)pCmdUI, (LPARAM)bCheck);
}

PLUGIN_EXPORT void PIMessageBox(CString str, UINT uType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();
	pMainApp->m_pMainWnd->MessageBox(str, pMainApp->m_pszAppName, uType);
}

PLUGIN_EXPORT void PILog(CString strLog)
{
	CString strAppName = AfxGetApp()->m_pszAppName;
	strLog = strAppName + _T("> ") + strLog;

	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();

	COPYDATASTRUCT copyData = {0};
	copyData.lpData = strLog.GetBuffer();
	copyData.cbData = strLog.GetLength();
	pMainApp->m_pMainWnd->SendMessage(WM_COPYDATA, (WPARAM)NULL, (LPARAM)&copyData);
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
	CWinApp* pMainApp = pApp->GetMainApp();

	CDocument* pActiveDoc = NULL;
	pMainApp->m_pMainWnd->SendMessage(WM_GET_ACTIVE_DOC, (WPARAM)&pActiveDoc, (LPARAM)NULL);
	
	return pActiveDoc;
}

PLUGIN_EXPORT CView* PIGetActiveView()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CPluginSupportApp* pApp = (CPluginSupportApp*)AfxGetApp();
	CWinApp* pMainApp = pApp->GetMainApp();

	CView* pActiveView = NULL;
	pMainApp->m_pMainWnd->SendMessage(WM_GET_ACTIVE_VIEW, (WPARAM)&pActiveView, (LPARAM)NULL);

	return pActiveView;
}