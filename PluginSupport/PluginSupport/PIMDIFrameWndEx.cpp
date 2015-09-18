// PIMDIFrameWndEx.cpp : 实现文件
//

#include "stdafx.h"
#include "PIMDIFrameWndEx.h"
#include "PluginImpl.h"
#include "PIDockablePane.h"

// CPIMDIFrameWndEx

IMPLEMENT_DYNCREATE(CPIMDIFrameWndEx, CMDIFrameWndEx)

CPIMDIFrameWndEx::CPIMDIFrameWndEx()
	: m_nProgressType(PI_PROGRESS_THREAD_DLG)
	, m_bProgressThreadRunning(FALSE)
{

}

CPIMDIFrameWndEx::~CPIMDIFrameWndEx()
{
	for (int i=0; i<m_PaneArray.GetCount(); i++)
	{
		CPIDockablePane* pDockablePane = m_PaneArray.GetAt(i);
		delete pDockablePane;
	}
}

BEGIN_MESSAGE_MAP(CPIMDIFrameWndEx, CMDIFrameWndEx)
	ON_MESSAGE(WM_UI_ENABLE_EVENT, &CPIMDIFrameWndEx::OnUIEnableEvent)
	ON_MESSAGE(WM_UI_CHECK_EVENT, &CPIMDIFrameWndEx::OnUICheckEvent)
	ON_COMMAND_RANGE(PLUGIN_COMMAND_BEGIN, PLUGIN_COMMAND_END, &CPIMDIFrameWndEx::OnPluginCommand)
	ON_UPDATE_COMMAND_UI_RANGE(PLUGIN_COMMAND_BEGIN, PLUGIN_COMMAND_END, &CPIMDIFrameWndEx::OnUpdatePluginCommand)
	ON_MESSAGE(WM_GET_ACTIVE_DOC, &CPIMDIFrameWndEx::OnGetActiveDocument)
	ON_MESSAGE(WM_GET_ACTIVE_VIEW, &CPIMDIFrameWndEx::OnGetActiveView)
	ON_MESSAGE(WM_PROGRESS_INIT, &CPIMDIFrameWndEx::OnProgressInit)
	ON_MESSAGE(WM_PROGRESS_PERCENT, &CPIMDIFrameWndEx::OnProgressPercent)
	ON_MESSAGE(WM_PROGRESS_DONE, &CPIMDIFrameWndEx::OnProgressDone)
	ON_MESSAGE(WM_CREATE_DOCKABLE_PANE, &CPIMDIFrameWndEx::OnCreateDockablePane)
END_MESSAGE_MAP()

// CPIMDIFrameWndEx 消息处理程序

void CPIMDIFrameWndEx::ProgressInit(int nProgressType, LPCTSTR lpszText, CDialog** pDialog)
{
	m_nProgressType = nProgressType;
	m_bProgressThreadRunning = TRUE;
}

BOOL CPIMDIFrameWndEx::ProgressPercent(int nPercent)
{
	// return status of UI
	return m_bProgressThreadRunning;
}

void CPIMDIFrameWndEx::ProgressDone()
{
	m_bProgressThreadRunning = FALSE;
}

LRESULT CPIMDIFrameWndEx::OnUIEnableEvent(WPARAM wParam, LPARAM lParam)
{
	CCmdUI* pCmdUI = (CCmdUI*)wParam;
	ASSERT(pCmdUI != NULL);

	BOOL bEnable = (BOOL)lParam;
	pCmdUI->Enable(bEnable);

	return 0;
}

LRESULT CPIMDIFrameWndEx::OnUICheckEvent(WPARAM wParam, LPARAM lParam)
{
	CCmdUI* pCmdUI = (CCmdUI*)wParam;
	ASSERT(pCmdUI != NULL);

	BOOL bCheck = (BOOL)lParam;
	pCmdUI->SetCheck(bCheck);

	return 0;
}

void CPIMDIFrameWndEx::OnPluginCommand(UINT nID)
{
	PICommand(nID, NULL);
}

void CPIMDIFrameWndEx::OnUpdatePluginCommand(CCmdUI* pCmdUI)
{
	PICommand(pCmdUI->m_nID, pCmdUI);
}

LRESULT CPIMDIFrameWndEx::OnGetActiveDocument(WPARAM wParam, LPARAM lParam)
{
	CDocument** pActiveDoc = (CDocument**)wParam;

	// Get the active MDI child window
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild)
	{
		*pActiveDoc = pActiveChild->GetActiveDocument();
	}

	return 0;
}

LRESULT CPIMDIFrameWndEx::OnGetActiveView(WPARAM wParam, LPARAM lParam)
{
	CView** pActiveView = (CView**)wParam;

	// Get the active MDI child window
	CMDIChildWnd* pActiveChild = MDIGetActive();
	if (pActiveChild)
	{
		*pActiveView = pActiveChild->GetActiveView();
	}

	return 0;
}

LRESULT CPIMDIFrameWndEx::OnProgressInit(WPARAM wParam, LPARAM lParam)
{
	int nProgressType = int(wParam);
	LPCTSTR lpszText = (LPCTSTR)lParam;
	CDialog* pDialog = NULL;
	ProgressInit(nProgressType, lpszText, &pDialog);
	return (LRESULT)pDialog;
}

LRESULT CPIMDIFrameWndEx::OnProgressPercent(WPARAM wParam, LPARAM lParam)
{
	if (m_bProgressThreadRunning)
	{
		return ProgressPercent(int(lParam));
	}
	else
	{
		return m_bProgressThreadRunning;
	}
}

LRESULT CPIMDIFrameWndEx::OnProgressDone(WPARAM wParam, LPARAM lParam)
{
	ProgressDone();

	return 0;
}

LRESULT CPIMDIFrameWndEx::OnCreateDockablePane(WPARAM wParam, LPARAM lParam)
{
	CPluginWindow* pPluginWindow = (CPluginWindow*)wParam;
	HINSTANCE hInstance = pPluginWindow->hInstance;
	CWnd* pWnd = pPluginWindow->pWnd;
	LPCTSTR lpszCaption = (LPCTSTR)lParam;

	CPIDockablePane* pDockablePane = new CPIDockablePane(hInstance, pWnd);
	if (!pDockablePane->Create(lpszCaption, this, CRect(0, 0, 100, 100), TRUE, 150, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | CBRS_BOTTOM | CBRS_FLOAT_MULTI))
	{
		TRACE0("Failed to create dockable pane\n");
		return FALSE;		// fail to create
	}
	pDockablePane->EnableDocking(CBRS_ALIGN_ANY);
	DockPane(pDockablePane, AFX_IDW_DOCKBAR_RIGHT);
	m_PaneArray.Add(pDockablePane);

	return 0;
}