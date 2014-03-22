// PIToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginSupport.h"
#include "PIToolBar.h"
#include "PluginImpl.h"

#define AFX_TOOLBAR_BUTTON_MARGIN 6

// CPIToolBar

IMPLEMENT_DYNAMIC(CPIToolBar, CMFCToolBar)

CPIToolBar::CPIToolBar()
{

}

CPIToolBar::~CPIToolBar()
{

}

BEGIN_MESSAGE_MAP(CPIToolBar, CMFCToolBar)
END_MESSAGE_MAP()

#ifdef _DEBUG
void CPIToolBar::AssertValid() const
{
	CMFCToolBar::AssertValid();
}
#endif

// CPIToolBar 消息处理程序

INT_PTR CPIToolBar::OnToolHitTest(CPoint point, TOOLINFO* pTI) const
{
	// TODO: 在此添加专用代码和/或调用基类
	CString strText;

	//先调用基类函数,再修改串
	INT_PTR hInt = CMFCToolBar::OnToolHitTest(point, pTI);
	if (pTI->uId == 0)
	{
		INT_PTR nHit = (INT_PTR) CMFCBaseToolBar::OnToolHitTest(point, pTI);
		if (nHit == -1)
		{
			nHit = ((CMFCToolBar*)this)->HitTest(point);
			if (nHit != -1)
			{
				CMFCToolBarButton* pButton = GetButton((int)nHit);
				if (pButton != NULL)
				{
					if (pButton->m_nID >= PLUGIN_COMMAND_BEGIN && pButton->m_nID <= PLUGIN_COMMAND_END)
					{
						PIGetCommandString(pButton->m_nID, strText);
						GetItemRect((int)nHit, &pTI->rect);
						pTI->uId = (pButton->m_nID == (UINT) -1) ? 0 : pButton->m_nID;
						pTI->hwnd = m_hWnd;
						hInt = pTI->uId;
					}
				}
			}
		}
	}
	else
	{
		strText = pTI->lpszText;
	}

	// 基类中会释放这个指针pTI->lpszText,但是由于我们要修改这个串,
	// 所以原来的空间不一定够,所以要释放掉原来的,并重新申请,
	int nLen = strText.GetLength() * sizeof(TCHAR);
	TCHAR* pBuf = (TCHAR*)malloc(nLen + 10);
	memset(pBuf, 0, nLen + 10);
	_tcscpy_s(pBuf, strText.GetLength() + 2, strText);
	free(pTI->lpszText);
	pTI->lpszText = pBuf;

	return hInt;
}

BOOL CPIToolBar::LoadToolBarExtend(UINT uiResID, UINT nCommandIDIndex, UINT uiColdResID, UINT uiMenuResID, BOOL bLocked, UINT uiDisabledResID, UINT uiMenuDisabledResID, UINT uiHotResID)
{
	CMFCToolBarInfo params;

	params.m_uiColdResID = uiColdResID;
	params.m_uiHotResID = uiHotResID;
	params.m_uiDisabledResID = uiDisabledResID;
	params.m_uiMenuResID = uiMenuResID;
	params.m_uiMenuDisabledResID= uiMenuDisabledResID;

	return LoadToolBarExtend2(uiResID, nCommandIDIndex, params, bLocked);
}

BOOL CPIToolBar::LoadToolBarExtend2(UINT uiToolbarResID, UINT nCommandIDIndex, CMFCToolBarInfo& params, BOOL bLocked)
{
	struct CToolBarData
	{
		WORD wVersion;
		WORD wWidth;
		WORD wHeight;
		WORD wItemCount;

		WORD* items() { return(WORD*)(this+1); }
	};

	ASSERT_VALID(this);

	ENSURE(uiToolbarResID != 0);

	// determine location of the bitmap in resource fork:
	HINSTANCE hInst = AfxFindResourceHandle(MAKEINTRESOURCE(uiToolbarResID), RT_TOOLBAR);
	HRSRC hRsrc = ::FindResourceW(hInst, MAKEINTRESOURCEW(uiToolbarResID), (LPWSTR) RT_TOOLBAR);
	if (hRsrc == NULL)
		return FALSE;

	HGLOBAL hGlobal = LoadResource(hInst, hRsrc);
	if (hGlobal == NULL)
		return FALSE;

	CToolBarData* pData = (CToolBarData*)LockResource(hGlobal);
	if (pData == NULL)
		return FALSE;
	ASSERT(pData->wVersion == 1);

	UINT* pItems = new UINT[pData->wItemCount];
	ENSURE(pItems != NULL);

	CSize sizeImage(pData->wWidth, pData->wHeight);
	CSize sizeButton(pData->wWidth + AFX_TOOLBAR_BUTTON_MARGIN, pData->wHeight + AFX_TOOLBAR_BUTTON_MARGIN);

	BOOL bDontScaleImages = bLocked ? m_bDontScaleLocked : m_bDontScaleImages;

	if (!bDontScaleImages && GetGlobalData()->GetRibbonImageScale() != 1.)
	{
		double dblImageScale = GetGlobalData()->GetRibbonImageScale();
		sizeButton = CSize ((int)(.5 + sizeButton.cx * dblImageScale), (int)(.5 + sizeButton.cy * dblImageScale));
	}

	if (bLocked)
	{
		SetLockedSizes(sizeButton, sizeImage);
	}
	else if (!m_Images.IsScaled())
	{
		SetSizes(sizeButton, sizeImage);
	}

	BOOL bResult = TRUE;

	if (params.m_uiHotResID == 0) // Use toolbar resource as hot image
	{
		params.m_uiHotResID = uiToolbarResID;
	}

	if (m_uiOriginalResID != 0 || LoadBitmapEx(params, bLocked))
	{
		int iImageIndex = m_iImagesOffset;
		for (int i = 0; i < pData->wItemCount; i++)
		{
		//	pItems[i] = pData->items()[i];			// origin version
			pItems[i] = nCommandIDIndex + i;		// new version

			if (!bLocked && pItems [i] > 0)
			{
				m_DefaultImages.SetAt(pItems[i], iImageIndex ++);
			}
		}

		m_uiOriginalResID = uiToolbarResID;
		bResult = SetButtons(pItems, pData->wItemCount);

		if (!bResult)
		{
			m_uiOriginalResID = 0;
		}
	}

	delete[] pItems;

	UnlockResource(hGlobal);
	FreeResource(hGlobal);

	return bResult;
}

// edit image id
void CPIToolBar::RemapImage(UINT nCommandID, int nImageIndex)
{
	m_DefaultImages.SetAt(nCommandID, nImageIndex);
}