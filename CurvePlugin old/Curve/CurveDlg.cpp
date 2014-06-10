// CurveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Curve.h"
#include "CurveDlg.h"
#include "afxdialogex.h"

// CCurveDlg 对话框

IMPLEMENT_DYNAMIC(CCurveDlg, CDialogEx)

	CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/) : CDialogEx(CCurveDlg::IDD, pParent)
{
	m_pWndCurve = NULL;
}

CCurveDlg::~CCurveDlg()
{
	if(m_pWndCurve != NULL)       delete m_pWndCurve;
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialogEx)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CCurveDlg 消息处理程序


int CCurveDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDialogEx::OnCreate(lpCreateStruct) == -1)
		return -1;

	//曲线窗
	CRect rect;
	GetClientRect(&rect);
	m_pWndCurve = new CCurveWnd;
	m_pWndCurve->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, this, IDW_CURVE_WINDOW);

	return 0;
}


void CCurveDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (m_pWndCurve->GetSafeHwnd())
	{
		m_pWndCurve->MoveWindow(0, 0, cx, cy);
	}
}


BOOL CCurveDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//WindowSize
	int nDlgWidth = 365+18;
	int nDlgHeight = 375+36;
	::SetWindowPos(this->m_hWnd, HWND_TOP, 100, 100, nDlgWidth, nDlgHeight, SWP_SHOWWINDOW);
	//CenterWindow();

	return TRUE;
}

BOOL CCurveDlg::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
}


void CCurveDlg::OnPaint()
{
	CPaintDC dc(this);
	CRect rect;
	GetClientRect(&rect);
	dc.FillSolidRect(rect, RGB(240, 240, 240));
}