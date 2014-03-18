#include "stdafx.h"
#include "ImagePlugin.h"

PLUGIN_EXPORT void Init(CWinApp* pApp, int nPluginIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT(pApp != NULL);
}

PLUGIN_EXPORT void Release()
{

}

PLUGIN_EXPORT void Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("ImagePlugin");
	plugininfo.m_strBlurb = _T("Add Image DocTemplate");
	plugininfo.m_strHelp = _T("For Image MultiDocTemplate");
	plugininfo.m_strAuthor = _T("Rolin");
	plugininfo.m_strCopyRight = _T("Copyright Rolin");
	plugininfo.m_strDate = _T("2014.3.12");
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

PLUGIN_EXPORT void Draw(CDC* pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

}

PLUGIN_EXPORT int GetDocTemplateCount()
{
	return 1;
}

PLUGIN_EXPORT CPIMultiDocTemplate* GetDocTemplate(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CImagePluginApp* pApp = (CImagePluginApp*)AfxGetApp();
	return pApp->GetDocTemplate();
}