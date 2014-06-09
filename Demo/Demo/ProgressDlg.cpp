// ProgressDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Demo.h"
#include "ProgressDlg.h"
#include "afxdialogex.h"
#include "MainFrm.h"

// CProgressDlg 对话框

IMPLEMENT_DYNAMIC(CProgressDlg, CDialog)

CProgressDlg::CProgressDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProgressDlg::IDD, pParent)
{

}

CProgressDlg::~CProgressDlg()
{

}

void CProgressDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_ctrlProgress);
}

BEGIN_MESSAGE_MAP(CProgressDlg, CDialog)
	ON_WM_NCHITTEST()
END_MESSAGE_MAP()

// CProgressDlg 消息处理程序

BOOL CProgressDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_ctrlProgress.SetRange(0, 100);
	m_ctrlProgress.SetPos(0);

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
	// TODO: 在此添加专用代码和/或调用基类
	CDemoApp* pApp = (CDemoApp*)AfxGetApp();
	CMainFrame* pMainFrame = (CMainFrame*)pApp->m_pMainWnd;
	pMainFrame->SetPluginThreadRunning(FALSE);

	CDialog::OnCancel();
}

LRESULT CProgressDlg::OnNcHitTest(CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return HTCAPTION;
	return CDialog::OnNcHitTest(point);
}