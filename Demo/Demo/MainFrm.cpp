
// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "Demo.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CPIMDIFrameWndEx)

const int  iMaxUserToolbars = 10;
const UINT uiFirstUserToolBarId = AFX_IDW_CONTROLBAR_FIRST + 40;
const UINT uiLastUserToolBarId = uiFirstUserToolBarId + iMaxUserToolbars - 1;

BEGIN_MESSAGE_MAP(CMainFrame, CPIMDIFrameWndEx)
	ON_WM_CREATE()
	ON_COMMAND(ID_WINDOW_MANAGER, &CMainFrame::OnWindowManager)
	ON_COMMAND(ID_VIEW_CUSTOMIZE, &CMainFrame::OnViewCustomize)
	ON_REGISTERED_MESSAGE(AFX_WM_CREATETOOLBAR, &CMainFrame::OnToolbarCreateNew)
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_MESSAGE(WM_MENU_EVENT, &CMainFrame::OnMenuEvent)
	ON_MESSAGE(WM_TOOLBAR_EVENT, &CMainFrame::OnToolbarEvent)
	ON_COMMAND(ID_INDICATOR_PROGRESS, &CMainFrame::OnIndicatorProgress)
	ON_WM_CLOSE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_INDICATOR_PROGRESS,	// progress bar
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	theApp.m_nAppLook = theApp.GetInt(_T("ApplicationLook"), ID_VIEW_APPLOOK_OFF_2007_BLACK);
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CPIMDIFrameWndEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	BOOL bNameValid;

	CMDITabInfo mdiTabParams;
	mdiTabParams.m_style = CMFCTabCtrl::STYLE_3D_ONENOTE; // other styles available...
	mdiTabParams.m_bActiveTabCloseButton = TRUE;      // set to FALSE to place close button at right of tab area
	mdiTabParams.m_bTabIcons = FALSE;    // set to TRUE to enable document icons on MDI taba
	mdiTabParams.m_bAutoColor = TRUE;    // set to FALSE to disable auto-coloring of MDI tabs
	mdiTabParams.m_bDocumentMenu = TRUE; // enable the document menu at the right edge of the tab area
	EnableMDITabbedGroups(TRUE, mdiTabParams);

	if (!m_wndMenuBar.Create(this))
	{
		TRACE0("Failed to create menubar\n");
		return -1;      // fail to create
	}

	m_wndMenuBar.SetPaneStyle(m_wndMenuBar.GetPaneStyle() | CBRS_SIZE_DYNAMIC | CBRS_TOOLTIPS | CBRS_FLYBY);

	// prevent the menu bar from taking the focus on activation
	CMFCPopupMenu::SetForceMenuFocus(FALSE);

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(theApp.m_bHiColorIcons ? IDR_MAINFRAME_256 : IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	CString strToolBarName;
	bNameValid = strToolBarName.LoadString(IDS_TOOLBAR_STANDARD);
	ASSERT(bNameValid);
	m_wndToolBar.SetWindowText(strToolBarName);

	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);
	m_wndToolBar.EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);

	// Allow user-defined toolbars operations:
	InitUserToolbars(NULL, uiFirstUserToolBarId, uiLastUserToolBarId);

	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: Delete these five lines if you don't want the toolbar and menubar to be dockable
	m_wndMenuBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockPane(&m_wndMenuBar);
	DockPane(&m_wndToolBar);


	// enable Visual Studio 2005 style docking window behavior
	CDockingManager::SetDockingMode(DT_SMART);
	// enable Visual Studio 2005 style docking window auto-hide behavior
	EnableAutoHidePanes(CBRS_ALIGN_ANY);
	// set the visual manager and style based on persisted value
	OnApplicationLook(theApp.m_nAppLook);

	// Enable enhanced windows management dialog
	EnableWindowsDialog(ID_WINDOW_MANAGER, ID_WINDOW_MANAGER, TRUE);

	// Enable toolbar and docking window menu replacement
	EnablePaneMenu(TRUE, ID_VIEW_CUSTOMIZE, strCustomize, ID_VIEW_TOOLBAR);

	// enable quick (Alt+drag) toolbar customization
	CMFCToolBar::EnableQuickCustomization();

	if (CMFCToolBar::GetUserImages() == NULL)
	{
		// load user-defined toolbar images
		if (m_UserImages.Load(_T(".\\UserImages.bmp")))
		{
			CMFCToolBar::SetUserImages(&m_UserImages);
		}
	}

	// Switch the order of document name and application name on the window title bar. This
	// improves the usability of the taskbar because the document name is visible with the thumbnail.
	ModifyStyle(0, FWS_PREFIXTITLE);

	// Initialize StatusBar
	InitStatusBar();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CPIMDIFrameWndEx::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}

// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CPIMDIFrameWndEx::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CPIMDIFrameWndEx::Dump(dc);
}
#endif //_DEBUG


// CMainFrame message handlers

// Initialize StatusBar
void CMainFrame::InitStatusBar()
{
	m_wndStatusBar.SetPaneWidth(nStatusProgress, 100);
	
	m_wndStatusBar.SetPaneStyle(nStatusInfo, SBPS_STRETCH | SBPS_NOBORDERS);

	m_wndStatusBar.EnablePaneDoubleClick();
}

UINT ShowProgressDlgThread(LPVOID pParam)
{
	CProgressDlg* pProgressDlg = (CProgressDlg*)pParam;
	pProgressDlg->DoModal();

	return 0;
}

void CMainFrame::ProgressInit(BOOL bDlgOrBar, LPCTSTR lpszText, CDialog** pDialog)
{
	CPIMDIFrameWndEx::ProgressInit(bDlgOrBar, lpszText, pDialog);

	if (bDlgOrBar == PI_PROGRESS_DLG)
	{
		AfxBeginThread(ShowProgressDlgThread, &m_wndProgressDlg);
		*pDialog = &m_wndProgressDlg;
	}
	else if (bDlgOrBar == PI_PROGRESS_BAR)
	{
		m_wndStatusBar.SetTipText(nStatusProgress, lpszText);
		m_wndStatusBar.EnablePaneProgressBar(nStatusProgress, 100L, TRUE);
	}
}

BOOL CMainFrame::ProgressPercent(int nPercent)
{
	if (GetProgressType() == PI_PROGRESS_DLG)
	{
		if (m_wndProgressDlg.GetSafeHwnd())
		{
			m_wndProgressDlg.SetPercent(nPercent);
		}
	}
	else if (GetProgressType() == PI_PROGRESS_BAR)
	{
		m_wndStatusBar.SetPaneProgress(nStatusProgress, nPercent);
	}

	return CPIMDIFrameWndEx::ProgressPercent(nPercent);
}

void CMainFrame::ProgressDone()
{
	CPIMDIFrameWndEx::ProgressDone();

	if (GetProgressType() == PI_PROGRESS_DLG)
	{
		if (m_wndProgressDlg.GetSafeHwnd())
		{
			m_wndProgressDlg.SendMessage(WM_CLOSE);
		}
	}
	else if (GetProgressType() == PI_PROGRESS_BAR)
	{
		m_wndStatusBar.SetPaneProgress(nStatusProgress, 0);
		m_wndStatusBar.SetTipText(nStatusProgress, NULL);
	}
}

void CMainFrame::OnWindowManager()
{
	ShowWindowsDialog();
}

void CMainFrame::OnViewCustomize()
{
	CMFCToolBarsCustomizeDialog* pDlgCust = new CMFCToolBarsCustomizeDialog(this, TRUE /* scan menus */);
	pDlgCust->EnableUserDefinedToolbars();
	pDlgCust->Create();
}

LRESULT CMainFrame::OnToolbarCreateNew(WPARAM wp,LPARAM lp)
{
	LRESULT lres = CPIMDIFrameWndEx::OnToolbarCreateNew(wp,lp);
	if (lres == 0)
	{
		return 0;
	}

	CMFCToolBar* pUserToolbar = (CMFCToolBar*)lres;
	ASSERT_VALID(pUserToolbar);

	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
	return lres;
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(NULL, NULL, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

	theApp.WriteInt(_T("ApplicationLook"), theApp.m_nAppLook);
}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

BOOL CMainFrame::LoadFrame(UINT nIDResource, DWORD dwDefaultStyle, CWnd* pParentWnd, CCreateContext* pContext) 
{
	// base class does the real work

	if (!CPIMDIFrameWndEx::LoadFrame(nIDResource, dwDefaultStyle, pParentWnd, pContext))
	{
		return FALSE;
	}

	// enable customization button for all user toolbars
	BOOL bNameValid;
	CString strCustomize;
	bNameValid = strCustomize.LoadString(IDS_TOOLBAR_CUSTOMIZE);
	ASSERT(bNameValid);

	for (int i = 0; i < iMaxUserToolbars; i ++)
	{
		CMFCToolBar* pUserToolbar = GetUserToolBarByIndex(i);
		if (pUserToolbar != NULL)
		{
			pUserToolbar->EnableCustomizeButton(TRUE, ID_VIEW_CUSTOMIZE, strCustomize);
		}
	}

	return TRUE;
}

void CMainFrame::OnClose()
{
	CDemoApp* pApp = (CDemoApp*)AfxGetApp();
	pApp->RemovePluginDocTemplate();

	CPIMDIFrameWndEx::OnClose();
}

LRESULT CMainFrame::OnMenuEvent(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;
	ASSERT(lParam != NULL);

	if (bFlag)
	{
		// create menu
		CMenu* pMenu = (CMenu*)lParam;
		m_wndMenuBar.CreateFromMenu(*pMenu, FALSE, TRUE);
	}
	else
	{
		// return menu
		HMENU* pMenu = (HMENU*)lParam;
		*pMenu = m_wndMenuBar.GetHMenu();
	}

	return 0;
}

LRESULT CMainFrame::OnToolbarEvent(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;
	ToolbarInfo* pInfo = (ToolbarInfo*)lParam;
	ASSERT(pInfo != NULL);

	// save current resource handle
	HINSTANCE hCurrentInstance = AfxGetResourceHandle();
	AfxSetResourceHandle(pInfo->hInstance);

	int nCommandCount = 0;
	if (bFlag)
	{
		// merge toolbar
		CPIToolBar PluginToolBar;
	//	if (PluginToolBar.LoadToolBarExtend(pInfo->nIDResource, pInfo->nCommandIDIndex - 4))
		if (PluginToolBar.LoadToolBar(pInfo->nIDResource))
		{
			nCommandCount = PluginToolBar.GetCount();
			for (int i=0; i<nCommandCount; i++)
			{
				UINT nID;
				UINT nStyle;
				int iImage;
				PluginToolBar.GetButtonInfo(i, nID, nStyle, iImage);

				// add new command
				int nVirtualID = PIAddNewCommand(pInfo->nPluginIndex, nID);
				if (nVirtualID)
				{
					CString str;
					str.LoadString(pInfo->hInstance, nID);
					CMFCToolBarButton ToolBarButton(nVirtualID, iImage, str);
					if (m_wndToolBar.InsertButton(ToolBarButton) == -1)
					{
						TRACE(_T("Plugin: InsertButton id = %d failed!\n"), nID);
					}

				//	PluginToolBar.RemapImage(nVirtualID, iImage);
				//	m_wndToolBar.ResetImages()
				}
			}
			m_wndToolBar.RestoreOriginalState();
		}
	}
	else
	{
		// insert toolbar
		CPIToolBar* pToolBar = new CPIToolBar();
		if (!pToolBar->CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
			!pToolBar->LoadToolBar(pInfo->nIDResource))
		{
			TRACE(_T("Plugin: InsertToolbar resource = %d failed!\n"), pInfo->nIDResource);
			return -1;
		}
		nCommandCount = pToolBar->GetCount();
		for (int i=0; i<nCommandCount; i++)
		{
			UINT nID;
			UINT nStyle;
			int iImage;
			pToolBar->GetButtonInfo(i, nID, nStyle, iImage);

			// replace command id
			pToolBar->SetButtonInfo(i, pInfo->nCommandIDIndex + i, nStyle, iImage);
			// set button text
			CString str;
			str.LoadString(pInfo->hInstance, nID);
			pToolBar->SetButtonText(i, str);
			// add new command
			PIAddNewCommand(pInfo->nPluginIndex, nID);
		}

		pToolBar->SetWindowText(pInfo->strText);
		pToolBar->EnableDocking(CBRS_ALIGN_ANY);

		//	DockPane(&m_wndToolBar);
		//	DockPaneLeftOf(pToolBar, &m_wndToolBar);
		DockPane(pToolBar);
		DockPaneLeftOf(&m_wndToolBar, pToolBar);

		// add to toolbar array
		m_ToolBarArray.Add(pToolBar);
	}

	// restore resource handle
	AfxSetResourceHandle(hCurrentInstance);

	return nCommandCount;
}

void CMainFrame::GetMessageString(UINT nID, CString& rMessage) const
{
	if (nID >= PLUGIN_COMMAND_BEGIN && nID <= PLUGIN_COMMAND_END)
	{
		PIGetCommandString(nID, rMessage);
		return;
	}

	return CPIMDIFrameWndEx::GetMessageString(nID, rMessage);
}

void CMainFrame::OnIndicatorProgress()
{
	SetProgressThreadRunning(FALSE);

	m_wndStatusBar.SetPaneProgress(nStatusProgress, 0);
	m_wndStatusBar.SetTipText(nStatusProgress, NULL);
}