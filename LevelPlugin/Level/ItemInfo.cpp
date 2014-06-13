#include "stdafx.h"
#include "ItemInfo.h"


CItemInfo::CItemInfo(const ItemType& eType, const ItemPosition& ePosition, const CRect& rect, const UINT& nID,
					 const UINT& nSourceID , const int& nSourceNum, const ItemExStyle& eExStyle,
					 int nMinIndex, BOOL bMinFlag, int nMinDis,	int nMaxIndex, BOOL bMaxFlag, int nMaxDis, CWnd* pItem)
{
	m_eType = eType;
	m_ePosition = ePosition;
	m_Rect = rect;
	m_nID = nID;
	m_nSourceID = nSourceID;
	m_nSourceNum = nSourceNum;
	m_eExStyle =eExStyle;
	m_nMinIndex = nMinIndex;
	m_bMinFlag = bMinFlag;
	m_nMinDis = nMinDis;
	m_nMaxIndex = nMaxIndex;
	m_bMaxFlag =bMaxFlag;
	m_nMaxDis = nMaxDis;
	m_pItem = pItem;
}

CItemInfo::~CItemInfo(void)
{
}
