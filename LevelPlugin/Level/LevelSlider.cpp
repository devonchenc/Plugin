// LevelSlider.cpp : 实现文件
//

#include "stdafx.h"
#include "Level.h"
#include "LevelSlider.h"


// CLevelSlider

IMPLEMENT_DYNAMIC(CLevelSlider, CWnd)

CLevelSlider::CLevelSlider()
{
	CPoint ptVertex[7]={CPoint(6,0), CPoint(12,6), CPoint(12,9), CPoint(11,10), CPoint(1,10), CPoint(0,9), CPoint(0,6)};//Wnd区域
	CPoint ptInnerVertex[5]={CPoint(6,1), CPoint(11,6), CPoint(11,9), CPoint(1,9), CPoint(1,6)};//内部区域
	//memmove(m_ptVertex, ptInnerVertex, sizeof(ptInnerVertex));
	m_rgn.CreatePolygonRgn(ptVertex,7,ALTERNATE);
	m_rgnInner.CreatePolygonRgn(ptInnerVertex,5,ALTERNATE);
}

CLevelSlider::~CLevelSlider()
{
}


BEGIN_MESSAGE_MAP(CLevelSlider, CWnd)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CLevelSlider 消息处理程序
void CLevelSlider::NewLevelSlider(CRect& rect, const UINT& nID, const ItemExStyle& eExStyle,CWnd* pParent, CLevelSlider* pItem, 
								  const CRect* pMin, const BOOL& bMinFlag, const int& nMinDis, const CRect* pMax, const BOOL& bMaxFlag, const int& nMaxDis)
{
	m_nID = nID;
	m_eExStyle = eExStyle;
	m_pPosition = &rect;
	m_pMin = pMin;
	m_bMinFlag = bMinFlag;
	m_nMinDis = nMinDis;
	m_pMax = pMax;
	m_bMaxFlag = bMaxFlag;
	m_nMaxDis = nMaxDis;
	pItem->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rect, pParent, nID);
	SetWindowRgn(m_rgn,TRUE);
}

COLORREF CLevelSlider::SetLevelSliderColor()
{
	switch (m_eExStyle)
	{
	case COL_BLACK:
		return RGB(0,0,0);
	case COL_GRAY:
		return RGB(170,170,170);
	case COL_WHITE:
		return RGB(255,255,255);
	default:
		return NULL;
	}
}

void CLevelSlider::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	CRect rect;
	GetClientRect(&rect); // 得到按钮窗口的有效矩形区域
	CDC MemDC;//创建内存DC
	MemDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	MemDC.SelectObject(&bitmap);
	MemDC.FillSolidRect(&rect, RGB(150,150,150));
	CBrush brush(SetLevelSliderColor());
	MemDC.FillRgn(&m_rgnInner, &brush);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &MemDC, 0, 0, SRCCOPY);
	bitmap.DeleteObject();
	MemDC.DeleteDC();
	// 不为绘图消息调用 CWnd::OnPaint()
}

void CLevelSlider::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	SetCapture();
	CWnd::OnLButtonDown(nFlags, point);
}

void CLevelSlider::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	ReleaseCapture();
	CLevelWnd* pLevelWnd = (CLevelWnd*)this->GetParent(); 
	pLevelWnd->GrayMapping();
	pLevelWnd->UpdateImage();
	CWnd::OnLButtonUp(nFlags, point);
}

void CLevelSlider::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (GetCapture() == this)
	{
		CRect SliderRect;
		GetWindowRect(&SliderRect); // 得到按钮客户区域的屏幕坐标位置
		CPoint CursorRect;
		GetCursorPos(&CursorRect); // 得到鼠标的屏幕坐标位置
		int nMoveDistance = int(CursorRect.x-SliderRect.left-0.5*SliderRect.Width());
		LONG lMinCenterPosition = m_bMinFlag==TRUE?LONG(m_pMin->left+0.5*m_pMin->Width()+m_nMinDis):LONG(m_pMin->left+m_nMinDis);
		LONG lMaxCenterPosition = m_bMaxFlag==TRUE?LONG(m_pMax->left+0.5*m_pMax->Width()-m_nMaxDis):LONG(m_pMax->right-m_nMaxDis);
		int nWidth = m_pPosition->Width();
		LONG lCenterPosition = LONG(m_pPosition->left+0.5*nWidth);
		if ((lCenterPosition+nMoveDistance)<lMinCenterPosition)
		{
			m_pPosition->left = LONG(lMinCenterPosition-0.5*nWidth);
			m_pPosition->right = LONG(lMinCenterPosition+0.5*nWidth);
			lCenterPosition = lMinCenterPosition;
		}
		else if ((lCenterPosition+nMoveDistance)>lMaxCenterPosition)
		{
			m_pPosition->left = LONG(lMaxCenterPosition-0.5*nWidth);
			m_pPosition->right = LONG(lMaxCenterPosition+0.5*nWidth);
			lCenterPosition = lMaxCenterPosition;
		}
		else
		{
			m_pPosition->left = (m_pPosition->left)+nMoveDistance;
			m_pPosition->right = (m_pPosition->right)+nMoveDistance;
			lCenterPosition+=nMoveDistance;
		}
		MoveWindow(m_pPosition);
		CLevelWnd* pLevelWnd = (CLevelWnd*)this->GetParent(); 
		pLevelWnd->CoordinateToGray(m_nID, lCenterPosition);
	}
	Invalidate();
	CWnd::OnMouseMove(nFlags, point);
}
