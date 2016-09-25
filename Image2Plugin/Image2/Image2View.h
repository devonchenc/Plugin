#pragma once

class CImage2Document;

// CImageView ��ͼ

class CImage2View : public CScrollView
{
	DECLARE_DYNCREATE(CImage2View)

protected:
	CImage2View();           // ��̬������ʹ�õ��ܱ����Ĺ��캯��
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
	virtual void OnDraw(CDC* pDC);      // ��д�Ի��Ƹ���ͼ
	virtual void OnInitialUpdate();     // �����ĵ�һ��

	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // CImageView.cpp �еĵ��԰汾
inline CImage2Document* CImage2View::GetDocument() const
{ return reinterpret_cast<CImage2Document*>(m_pDocument); }
#endif
