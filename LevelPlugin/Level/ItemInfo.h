#pragma once
enum ItemType{LEVEL_BUTTON = 300,  LEVEL_COMBOBOX,  LEVEL_TEXT,  LEVEL_BAR,  LEVEL_HISTOGRAM, LEVEL_EDIT_NUM, LEVEL_EDIT_CTRL, LEVEL_SLDWND};
enum ItemPosition{LEFT_STATIC = 400, RIGHT_STATIC, BOTTOM_STATIC, LEFT_CHANGE, BOTTOM_CHANGE,BOTTOM_MEDIAN, BOTTOM_RIGHT};
enum ItemExStyle{NONE=500, COL_BLACK, COL_GRAY, COL_WHITE};


class CItemInfo
{
public:
	CItemInfo(const ItemType& eType, const ItemPosition& ePosition, const CRect& rect, const UINT& nID = 0, 
		const UINT& nSourceID = 0, const int& nSourceNum = 0, const ItemExStyle& eExStyle = NONE, 
		int nMinIndex = 0, BOOL bMinFlag=FALSE, int nMinDis=0, int nMaxIndex = 0,BOOL bMaxFlag = FALSE, int nMaxDis = 0, CWnd* pItem = NULL);
	~CItemInfo(void);

public:
	ItemType m_eType;
	ItemPosition m_ePosition;
	CRect m_Rect;
	UINT m_nID;
	UINT m_nSourceID;
	int m_nSourceNum;
	ItemExStyle m_eExStyle;
	int m_nMinIndex;
	BOOL m_bMinFlag;
	int m_nMinDis;
	int m_nMaxIndex;
	BOOL m_bMaxFlag;
	int m_nMaxDis;
	CWnd* m_pItem;
};

