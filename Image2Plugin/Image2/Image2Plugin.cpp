#include "stdafx.h"
#include "Image2Plugin.h"
#include "Image2.h"

IMPLEMENT_PLUGIN(CImage2Plugin)

void CImage2Plugin::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_IMAGE_MENU);
	MergeMenu(&append, TRUE);

	// merge toolbar
//	MergeToolbar(IDR_DRAW_TOOLBAR);
}

void CImage2Plugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("Image2Plugin");
	plugininfo.m_strBlurb = _T("Add Image2 DocTemplate");
	plugininfo.m_strHelp = _T("For Image2 MultiDocTemplate");
	plugininfo.m_strAuthor = _T("wyc");
	plugininfo.m_strCopyRight = _T("Copyright wyc");
	plugininfo.m_strDate = _T("2016.09.25");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}

int CImage2Plugin::GetDocTemplateCount()
{
	return 1;
}

CPIMultiDocTemplate* CImage2Plugin::GetDocTemplate(int nIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CImage2App* pApp = (CImage2App*)AfxGetApp();
	return pApp->GetDocTemplate();
}