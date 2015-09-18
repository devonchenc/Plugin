#include "stdafx.h"
#include "ImageProcessingPlugin.h"
#include "Process.h"

IMPLEMENT_PLUGIN(CImageProcessingPlugin)

void CImageProcessingPlugin::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_PROCESS_MENU);
	MergeMenu(&append, TRUE);
}

void CImageProcessingPlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("ImageProcessingPlugin");
	plugininfo.m_strBlurb = _T("Process Image");
	plugininfo.m_strHelp = _T("For Image Processing");
	plugininfo.m_strAuthor = _T("Rolin");
	plugininfo.m_strCopyRight = _T("Copyright Rolin");
	plugininfo.m_strDate = _T("2014.6.9");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}