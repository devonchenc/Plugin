#include "stdafx.h"
#include "Browser.h"
#include "PluginBrowser.h"

IMPLEMENT_PLUGIN(CBrowserPlugin)

void CBrowserPlugin::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_BROWSER_MENU);
	MergeMenu(&append, TRUE);
}

void CBrowserPlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("PluginBrowser");
	plugininfo.m_strBlurb = _T("Show Plugin Information");
	plugininfo.m_strHelp = _T("PluginBrowser");
	plugininfo.m_strAuthor = _T("Zhao Shusen");
	plugininfo.m_strCopyRight = _T("Copyright Rolin");
	plugininfo.m_strDate = _T("2014.3.17");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}