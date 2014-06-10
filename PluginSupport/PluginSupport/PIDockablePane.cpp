// PIDockablePane.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginSupport.h"
#include "PIDockablePane.h"


// CPIDockablePane

IMPLEMENT_DYNAMIC(CPIDockablePane, CDockablePane)

CPIDockablePane::CPIDockablePane(HINSTANCE hInstance, CWnd* pChildWnd)
	: m_hInstance(hInstance)
	, m_pChildWnd(pChildWnd)
{

}

CPIDockablePane::~CPIDockablePane()
{

}


BEGIN_MESSAGE_MAP(CPIDockablePane, CDockablePane)
	ON_WM_CREATE()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CPIDockablePane 消息处理程序

int CPIDockablePane::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CDockablePane::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  在此添加您专用的创建代码
	CRect rectDummy;
	rectDummy.SetRectEmpty();

	// create child window
	if (m_pChildWnd->GetSafeHwnd() == NULL)
	{
		// save current resource handle
		HINSTANCE hCurrentInstance = AfxGetResourceHandle();
		AfxSetResourceHandle(m_hInstance);
		m_pChildWnd->Create(NULL, NULL, WS_CHILD | WS_VISIBLE, rectDummy, this, 1000);

		// restore resource handle
		AfxSetResourceHandle(hCurrentInstance);
	}

	return 0;
}

void CPIDockablePane::OnSize(UINT nType, int cx, int cy)
{
	CDockablePane::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	m_pChildWnd->SetWindowPos(NULL, -1, -1, cx, cy, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOZORDER);
}
