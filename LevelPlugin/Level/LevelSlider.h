#pragma once
#include "ItemInfo.h"
#include "LevelWnd.h"

// CLevelSlider

class CLevelSlider : public CWnd
{
	DECLARE_DYNAMIC(CLevelSlider)

public:
	CLevelSlider();
	virtual ~CLevelSlider();

protected:
	DECLARE_MESSAGE_MAP()
public:
	void NewLevelSlider(CRect& rect, const UINT& nID, const ItemExStyle& eExStyle, CWnd* pParent,
		CLevelSlider* pItem, const CRect* pMin, const BOOL& bMinFlag, const int& nMinDis, const CRect* pMax, const BOOL& bMaxFlag, const int& nMaxDis);
	COLORREF SetLevelSliderColor();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	UINT m_nID;
	CRgn m_rgn;
	CRgn m_rgnInner;
	ItemExStyle m_eExStyle;
	CRect* m_pPosition;
	const CRect* m_pMin;
	BOOL m_bMinFlag;
	int m_nMinDis;
	const CRect* m_pMax;
	BOOL m_bMaxFlag;
	int m_nMaxDis;
};


