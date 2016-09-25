// ImageView.cpp : 实现文件
//

#include "stdafx.h"
#include "Image2.h"
#include "Image2View.h"

#include "Image2Document.h"

// CImageView

IMPLEMENT_DYNCREATE(CImage2View, CScrollView)

CImage2View::CImage2View()
{

}

CImage2View::~CImage2View()
{

}

BEGIN_MESSAGE_MAP(CImage2View, CScrollView)
END_MESSAGE_MAP()

// CImageView 绘图

void CImage2View::OnInitialUpdate()
{
	CScrollView::OnInitialUpdate();

	CSize sizeTotal;

	CImage2Document* pDoc = GetDocument();
	CImage* pImage = pDoc->GetImage();
	if (pImage)
	{
		sizeTotal.cx = pImage->GetWidth();
		sizeTotal.cy = pImage->GetHeight();
	}
	else
	{
		sizeTotal.cx = sizeTotal.cy = 100;
	}
	SetScrollSizes(MM_TEXT, sizeTotal);
}

void CImage2View::OnDraw(CDC* pDC)
{
	CImage2Document* pDoc = GetDocument();
	CImage* pImage = pDoc->GetImage();
	if (pImage)
	{
		pImage->Draw(pDC->GetSafeHdc(), 0, 0);
	}
}

// CImageView 诊断

#ifdef _DEBUG
void CImage2View::AssertValid() const
{
	CScrollView::AssertValid();
}

#ifndef _WIN32_WCE
void CImage2View::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}
#endif

CImage2Document* CImage2View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CImage2Document)));
	return (CImage2Document*)m_pDocument;
}
#endif //_DEBUG

// CImageView 消息处理程序
