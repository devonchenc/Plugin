#include "stdafx.h"
#include "ImagePlugin.h"
#include "Image.h"

IMPLEMENT_PLUGIN(CImagePlugin)

void CImagePlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("ImagePlugin");
	plugininfo.m_strBlurb = _T("Add Image DocTemplate");
	plugininfo.m_strHelp = _T("For Image MultiDocTemplate");
	plugininfo.m_strAuthor = _T("Rolin");
	plugininfo.m_strCopyRight = _T("Copyright Rolin");
	plugininfo.m_strDate = _T("2014.3.20");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}

int CImagePlugin::GetDocTemplateCount()
{
	return 1;
}

CPIMultiDocTemplate* CImagePlugin::GetDocTemplate(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CImageApp* pApp = (CImageApp*)AfxGetApp();
	return pApp->GetDocTemplate();
}