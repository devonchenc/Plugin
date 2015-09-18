// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialogEx)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{

}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialogEx)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CProgressDlg 消息处理程序

BOOL CProgressDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);

	if (m_strCaption.IsEmpty())
	{
		// remove title bar
		ModifyStyle(WS_CAPTION, 0);

		CRect rect;
		GetWindowRect(rect);
		SetWindowPos(NULL, 0, 0, rect.Width(), rect.Height() - 32, SWP_NOMOVE | SWP_NOZORDER | SWP_FRAMECHANGED);
	}
	else
	{
		SetWindowText(m_strCaption);
		m_strCaption.Empty();
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CProgressDlg::SetPercent(int nPercent)
{
	if (nPercent <= 100)
	{
		m_ctrlProgress.SetPos(nPercent);
	}
	else
	{
		SendMessage(WM_CLOSE);
	}
}

void CProgressDlg::OnCancel()
{
	CDemoApp* pApp = (CDemoApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	pMainFrame->SetProgressThreadRunning(FALSE);

	CDialogEx::OnCancel();
}

LRESULT CProgressDlg::OnNcHitTest(CPoint point)
{
	return HTCAPTION;
	return CDialogEx::OnNcHitTest(point);
}