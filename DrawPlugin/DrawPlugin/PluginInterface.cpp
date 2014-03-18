#include "stdafx.h"
#include "DrawPlugin.h"

PLUGIN_EXPORT void Init(CWinApp* pApp, int nPluginIndex)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	ASSERT(pApp != NULL);

	CDrawPluginApp* pThisApp = (CDrawPluginApp*)AfxGetApp();
	pThisApp->m_nPluginIndex = nPluginIndex;

	// merge menu
	CMenu append;
	append.LoadMenu(IDR_DRAW_MENU);
	MergeMenu(nPluginIndex, &append, TRUE);

	// merge toolbar
	MergeToolbar(nPluginIndex, IDR_DRAW_TOOLBAR);

//	// insert toolbar
//	CString str;
//	str.LoadString(IDS_STRING_TOOLBAR_NAME);
//	InsertToolbar(nPluginIndex, IDR_TOOLBAR1, str);
}

PLUGIN_EXPORT void Release()
{

}

PLUGIN_EXPORT void Query(CPluginInfo& plugininfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	plugininfo.m_strName = _T("Draw Plugin");
	plugininfo.m_strBlurb = _T("Hello, world!");
	plugininfo.m_strHelp = _T("For Test Merge Menu & Merge Toolbar");
	plugininfo.m_strAuthor = _T("Wang Hao");
	plugininfo.m_strCopyRight = _T("Copyright WangHao");
	plugininfo.m_strDate = _T("2014.2.14");
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

#define PEN_WIDTH 2
PLUGIN_EXPORT void Draw(CDC* pDC)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CDrawPluginApp* pApp = (CDrawPluginApp*)AfxGetApp();
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

		pDC->BitBlt(0,0,rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

		bitmap.DeleteObject();
		memDC.DeleteDC();
	}
}