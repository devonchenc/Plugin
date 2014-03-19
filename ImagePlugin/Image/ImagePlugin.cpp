#include "stdafx.h"
#include "ImagePlugin.h"
#include "Image.h"

CImagePlugin::CImagePlugin()
{

}

void CImagePlugin::Init(CWinApp* pApp, int nPluginIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
}

void CImagePlugin::Release()
{

}

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

void CImagePlugin::Draw(CDC* pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

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

PLUGIN_EXPORT CImagePlugin* GetInstance()
{
	CImagePlugin* pInstance = CImagePlugin::Instance();
	return pInstance;
}