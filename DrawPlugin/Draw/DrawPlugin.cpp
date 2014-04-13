#include "stdafx.h"
#include "DrawPlugin.h"
#include "Draw.h"

CDrawPlugin::CDrawPlugin()
{

}

void CDrawPlugin::Init()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_DRAW_MENU);
	MergeMenu(&append, TRUE);

	// merge toolbar
	MergeToolbar(IDR_DRAW_TOOLBAR);
}

void CDrawPlugin::Release()
{

}

void CDrawPlugin::Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("Draw Plugin");
	plugininfo.m_strBlurb = _T("Hello, world!");
	plugininfo.m_strHelp = _T("For Test Merge Menu & Merge Toolbar");
	plugininfo.m_strAuthor = _T("Wang Hao");
	plugininfo.m_strCopyRight = _T("Copyright WangHao");
	plugininfo.m_strDate = _T("2014.3.19");
	CString str;
	str.LoadString(IDS_STRING_MENU_LABEL);
	plugininfo.m_strMenuLabel = str;
}

#define PEN_WIDTH 2
void CDrawPlugin::Draw(CDC* pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDrawApp* pApp = (CDrawApp*)AfxGetApp();
	CWnd* pWnd = pDC->GetWindow();
	CShape* pShape = pApp->GetCurrentShape(pWnd);

	if (pApp->m_shpFlag != CShape::NONE && pShape)
	{
		CRect rect;
		pApp->GetMainWnd()->GetClientRect(&rect);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);

		CBitmap bitmap;
		bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());

		memDC.SelectObject(&bitmap);
		memDC.FillSolidRect(&rect, GetSysColor(COLOR_WINDOW));

		CPen pen;
		if (!pen.CreatePen(PS_SOLID, PEN_WIDTH, RGB(0,0,0)))
		{
			return;
		}
		CPen* pOldPen = memDC.SelectObject(&pen);

		pShape->Draw(&memDC);

		memDC.SelectObject(pOldPen);

		pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

		bitmap.DeleteObject();
		memDC.DeleteDC();
	}
}

int CDrawPlugin::GetDocTemplateCount()
{
	return 0;
}

CPIMultiDocTemplate* CDrawPlugin::GetDocTemplate(int nIndex)
{
	return NULL;
}

PLUGIN_EXPORT CDrawPlugin* GetInstance()
{
	CDrawPlugin* pInstance = CDrawPlugin::Instance();
	return pInstance;
}