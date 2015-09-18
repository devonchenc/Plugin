#include "stdafx.h"
#include "LevelPlugin.h"
#include "Level.h"

IMPLEMENT_PLUGIN(CLevelPlugin)

void CLevelPlugin::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_LEVEL_MENU);
	MergeMenu(&append, TRUE);
}

void CLevelPlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("LevelPlugin");
	plugininfo.m_strBlurb = _T("Process Image");
	plugininfo.m_strHelp = _T("For Image Processing");
	plugininfo.m_strAuthor = _T("Wangqian");
	plugininfo.m_strCopyRight = _T("Copyright Wangqian");
	plugininfo.m_strDate = _T("2015.9.18");
//	CString str;
//	str.LoadString(IDS_STRING_MENU_LABEL);
//	plugininfo.m_strMenuLabel = str;
}