// LevelDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Level.h"
#include "resource.h"
#include "LevelDlg.h"
#include "afxdialogex.h"


// CLevelDlg 对话框

IMPLEMENT_DYNAMIC(CLevelDlg, CDialogEx)

CLevelDlg::CLevelDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLevelDlg::IDD, pParent)
{

}

CLevelDlg::~CLevelDlg()
{
}

void CLevelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLevelDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CLevelDlg 消息处理程序

int CLevelDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect Rect;
	GetClientRect(&Rect);
	if (m_wndLevel.GetSafeHwnd()==NULL)
	{
		m_wndLevel.Create(NULL, NULL, WS_CHILD | WS_VISIBLE, Rect, this, IDW_LEVEL);
	}
	return 0;
}

BOOL CLevelDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();


	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CLevelDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect Rect;
	GetClientRect(&Rect);
	m_wndLevel.ReSize(nType, Rect);
}

BOOL CLevelDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

//	return CDialogEx::OnEraseBkgnd(pDC);
	return TRUE;
}
