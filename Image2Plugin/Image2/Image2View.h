#pragma once

class CImage2Document;

// CImageView 视图

class CImage2View : public CScrollView
{
	DECLARE_DYNCREATE(CImage2View)

protected:
	CImage2View();           // 动态创建所使用的受保护的构造函数
	virtual ~CImage2View();

public:
	CImage2Document* GetDocument() const;

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
inline CImage2Document* CImage2View::GetDocument() const
{ return reinterpret_cast<CImage2Document*>(m_pDocument); }
#endif
