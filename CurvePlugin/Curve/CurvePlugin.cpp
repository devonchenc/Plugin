#include "stdafx.h"
#include "CurvePlugin.h"
#include "Curve.h"

CCurvePlugin::CCurvePlugin()
{

}

void CCurvePlugin::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_CURVE_MENU);
	MergeMenu(&append, TRUE);
}

void CCurvePlugin::Release()
{

}

void CCurvePlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("CurvePlugin");
	plugininfo.m_strBlurb = _T("");
	plugininfo.m_strHelp = _T("For Image Processing");
	plugininfo.m_strAuthor = _T("");
	plugininfo.m_strCopyRight = _T("Copyright ");
	plugininfo.m_strDate = _T("2014.6.12");
	CString str;
//	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}

void CCurvePlugin::Draw(CDC* pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

}

int CCurvePlugin::GetDocTemplateCount()
{
	return 0;
}

CPIMultiDocTemplate* CCurvePlugin::GetDocTemplate(int nIndex)
{
	return NULL;
}

PLUGIN_EXPORT CCurvePlugin* GetInstance()
{
	CCurvePlugin* pInstance = CCurvePlugin::Instance();
	return pInstance;
}