// CurveWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "Curve.h"
#include "CurveWnd.h"


// CCurveWnd

IMPLEMENT_DYNAMIC(CCurveWnd, CWnd)

CCurveWnd::CCurveWnd()
{

}

CCurveWnd::~CCurveWnd()
{

}


BEGIN_MESSAGE_MAP(CCurveWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_BN_CLICKED(1001, OnTestButton)
END_MESSAGE_MAP()


// CCurveWnd 消息处理程序

int CCurveWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	CRect rectButton(50, 40, 120, 70);
	m_ctrlButton.Create(_T("Test"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, rectButton, this, 1001);

	return 0;
}

void CCurveWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	dc.MoveTo(5, 10);
	dc.LineTo(50, 80);
}

void CCurveWnd::OnTestButton()
{
	AfxMessageBox(_T("test"));
}