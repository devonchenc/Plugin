#include "stdafx.h"
#include "PluginBrowser.h"

PLUGIN_EXPORT void Init(CWinApp* pApp, int nPluginIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT(pApp != NULL);

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_BROWSER_MENU);
	MergeMenu(nPluginIndex, &append, TRUE);
}

PLUGIN_EXPORT void Release()
{

}

PLUGIN_EXPORT void Query(CPluginInfo& plugininfo)
{
	plugininfo.m_strName = _T("PluginBrowser");
	plugininfo.m_strBlurb = _T("Show Plugin Information");
	plugininfo.m_strHelp = _T("PluginBrowser");
	plugininfo.m_strAuthor = _T("Rolin");
	plugininfo.m_strCopyRight = _T("Copyright Rolin");
	plugininfo.m_strDate = _T("2014.3.17");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}

PLUGIN_EXPORT void Execute(UINT nCommandID, CCmdUI* pCmdUI)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CWinApp* pApp = AfxGetApp();
	if (pCmdUI == NULL)
	{
		pApp->OnCmdMsg(nCommandID, CN_COMMAND, NULL, NULL);
	}
	else
	{
		pApp->OnCmdMsg(nCommandID, CN_UPDATE_COMMAND_UI, pCmdUI, NULL);
	}
}