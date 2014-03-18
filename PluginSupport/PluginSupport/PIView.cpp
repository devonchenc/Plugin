// PIView.cpp : 实现文件
//

#include "stdafx.h"
#include "PIView.h"
#include "PluginImpl.h"

// CPIView

IMPLEMENT_DYNCREATE(CPIView, CScrollView)

CPIView::CPIView()
{

}

CPIView::~CPIView()
{

}

BEGIN_MESSAGE_MAP(CPIView, CScrollView)
	ON_MESSAGE(WM_PREPARE_DC_EVENT, &CPIView::OnPrepareDCEvent)
END_MESSAGE_MAP()

// CPIView 绘图

void CPIView::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;
	// TODO: 计算此视图的合计大小
	sizeTotal.cx = sizeTotal.cy = 300;
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CPIView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 在此添加绘制代码
	PIDraw(pDC);
}

// CPIView 诊断

#ifdef _DEBUG
void CPIView::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CPIView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif
#endif //_DEBUG

// CPIView 消息处理程序

BOOL CPIView::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	BOOL bResult = PIHandleMessage(pMsg, this);
	if (bResult)
	{
		return TRUE;
	}
	else
	{
		return CScrollView::PreTranslateMessage(pMsg);
	}
}

LRESULT CPIView::OnPrepareDCEvent(WPARAM wParam, LPARAM lParam)
{
	CDC* pDC = (CDC*)lParam;
	ASSERT(pDC != NULL);
	OnPrepareDC(pDC);

	return 0;
}