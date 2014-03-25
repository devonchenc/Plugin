// ListViewPage.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginBrowser.h"
#include "ListViewPage.h"
#include "afxdialogex.h"

#include "PluginBrowserDlg.h"

// CListViewPage 对话框

IMPLEMENT_DYNAMIC(CListViewPage, CDialogEx)

CListViewPage::CListViewPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CListViewPage::IDD, pParent)
{

}

CListViewPage::~CListViewPage()
{

}

void CListViewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGIN_LIST, m_ctrlPluginList);
}


BEGIN_MESSAGE_MAP(CListViewPage, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_PLUGIN_LIST, &CListViewPage::OnNMClickPluginList)
END_MESSAGE_MAP()


// CListViewPage 消息处理程序

BOOL CListViewPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	m_ctrlPluginList.MoveWindow(rect);

	CString str;
	str.LoadString(IDS_STRING_LIST_TEXT1);
	m_ctrlPluginList.InsertColumn(0, str, LVCFMT_LEFT, 80, 0);
	str.LoadString(IDS_STRING_LIST_TEXT2);
	m_ctrlPluginList.InsertColumn(1, str, LVCFMT_LEFT, 100, 1);
	str.LoadString(IDS_STRING_LIST_TEXT3);
	m_ctrlPluginList.InsertColumn(2, str, LVCFMT_LEFT, 80, 2);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CListViewPage::DisplayPluginInfo(const PluginInfoArray& array)
{
	// clear all
	m_ctrlPluginList.DeleteAllItems();

	for (int i=0; i<array.GetSize(); i++)
	{
		CPluginInfo* pInfo = array.GetAt(i);
		m_ctrlPluginList.InsertItem(i, pInfo->m_strName);
		m_ctrlPluginList.SetItemText(i, 1, pInfo->m_strMenuLabel);
		m_ctrlPluginList.SetItemText(i, 2, pInfo->m_strDate);
	}
}

void CListViewPage::OnNMClickPluginList(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);

	//获取选中的行的序号
	int nItem = m_ctrlPluginList.GetSelectionMark();
	if (nItem == -1)
		return;

	CTabCtrl* pTabCtrl = (CTabCtrl*)GetParent();
	ASSERT(pTabCtrl);
	CPluginBrowserDlg* pDlg = (CPluginBrowserDlg*)pTabCtrl->GetParent();
	ASSERT(pDlg);
	pDlg->PrintPluginInfo(nItem);

	*pResult = 0;
}

void CListViewPage::OnOK()
{
	return;
}

void CListViewPage::OnCancel()
{
	return;
}