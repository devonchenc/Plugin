// PIToolBar.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginSupport.h"
#include "PIToolBar.h"
#include "PluginImpl.h"


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