
// MainFrm.h : interface of the CMainFrame class
//

#pragma once

#include "ProgressDlg.h"

typedef CArray<CMFCToolBar*, CMFCToolBar*> ToolBarArray;

class CMainFrame : public CPIMDIFrameWndEx
{
	DECLARE_DYNAMIC(CMainFrame)
public:
	CMainFrame();

// Attributes
public:

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL LoadFrame(UINT nIDResource, DWORD dwDefaultStyle = WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, CWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);

// Implementation
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CMFCMenuBar			m_wndMenuBar;
	CMFCToolBar			m_wndToolBar;
	CMFCStatusBar		m_wndStatusBar;
	CMFCToolBarImages	m_UserImages;

	CProgressDlg		m_wndProgressDlg;

private:
	// plugin toolbar
	ToolBarArray m_ToolBarArray;

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnWindowManager();
	afx_msg void OnViewCustomize();
	afx_msg LRESULT OnToolbarCreateNew(WPARAM wp, LPARAM lp);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	afx_msg void OnClose();
	afx_msg LRESULT OnMenuEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnToolbarEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressInit(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressPercent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnProgressDone(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:
	virtual void GetMessageString(UINT nID, CString& rMessage) const;
};


