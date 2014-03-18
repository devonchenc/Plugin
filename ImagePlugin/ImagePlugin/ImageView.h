#pragma once

class CImageDocument;

// CImageView 视图

class CImageView : public CScrollView
{
	DECLARE_DYNCREATE(CImageView)

protected:
	CImageView();           // 动态创建所使用的受保护的构造函数
	virtual ~CImageView();

public:
	CImageDocument* GetDocument() const;

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CImageView.cpp 中的调试版本
inline CImageDocument* CImageView::GetDocument() const
{ return reinterpret_cast<CImageDocument*>(m_pDocument); }
#endif
