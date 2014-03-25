// TreeViewPage.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginBrowser.h"
#include "TreeViewPage.h"
#include "afxdialogex.h"

#include "PluginBrowserDlg.h"

// CTreeViewPage 对话框

IMPLEMENT_DYNAMIC(CTreeViewPage, CDialogEx)

CTreeViewPage::CTreeViewPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTreeViewPage::IDD, pParent)
{

}

CTreeViewPage::~CTreeViewPage()
{

}

void CTreeViewPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PLUGIN_TREE, m_ctrlPluginTree);
}


BEGIN_MESSAGE_MAP(CTreeViewPage, CDialogEx)
	ON_NOTIFY(TVN_SELCHANGED, IDC_PLUGIN_TREE, &CTreeViewPage::OnTvnSelchangedPluginTree)
END_MESSAGE_MAP()


// CTreeViewPage 消息处理程序

void CTreeViewPage::DisplayPluginInfo(const PluginInfoArray& array)
{
	// clear all
	m_ctrlPluginTree.DeleteAllItems();
	m_PairArray.RemoveAll();

	CString str;
	str.LoadString(IDS_STRING_TREE_ROOT_NAME);
	m_hRootMenu = m_ctrlPluginTree.InsertItem(str);

	for (int i=0; i<array.GetSize(); i++)
	{
		CPluginInfo* pInfo = array.GetAt(i);

		HTREEITEM hChild = SearchChildItem(pInfo->m_strMenuLabel);
		if (hChild == NULL)
		{
			hChild = m_ctrlPluginTree.InsertItem(pInfo->m_strMenuLabel, m_hRootMenu);
		}
		m_ctrlPluginTree.InsertItem(pInfo->m_strName, hChild);

		PluginPair pair;
		pair.nIndex = i;
		pair.strName = pInfo->m_strName;
		m_PairArray.Add(pair);
	}
	m_ctrlPluginTree.Expand(m_hRootMenu, TVE_EXPAND);
}

HTREEITEM CTreeViewPage::SearchChildItem(CString strChildName)
{
	HTREEITEM hNextItem;
	HTREEITEM hChildItem = m_ctrlPluginTree.GetChildItem(m_hRootMenu);

	BOOL bAlreadyInsert = FALSE;
	while (hChildItem != NULL)
	{
		CString strName = m_ctrlPluginTree.GetItemText(hChildItem);
		if (strChildName.CompareNoCase(strName) == 0)
		{
			return hChildItem;
		}

		hNextItem = m_ctrlPluginTree.GetNextItem(hChildItem, TVGN_NEXT);
		hChildItem = hNextItem;
	}
	return NULL;
}

BOOL CTreeViewPage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	CRect rect;
	GetClientRect(&rect);
	m_ctrlPluginTree.MoveWindow(rect);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CTreeViewPage::OnTvnSelchangedPluginTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);

	HTREEITEM hSelected = m_ctrlPluginTree.GetSelectedItem();
	int nIndex = GetItemIndex(hSelected);
	if (nIndex == -1)
		return;

	CTabCtrl* pTabCtrl = (CTabCtrl*)GetParent();
	ASSERT(pTabCtrl);
	CPluginBrowserDlg* pDlg = (CPluginBrowserDlg*)pTabCtrl->GetParent();
	ASSERT(pDlg);
	pDlg->PrintPluginInfo(nIndex);

	*pResult = 0;
}

int CTreeViewPage::GetItemIndex(HTREEITEM hItem)
{
	CString strName = m_ctrlPluginTree.GetItemText(hItem);

	for (int i=0; i<m_PairArray.GetSize(); i++)
	{
		PluginPair pair = m_PairArray.GetAt(i);
		if (pair.strName.CompareNoCase(strName) == 0)
		{
			return i;
		}
	}
	return -1;
}

void CTreeViewPage::OnOK()
{
	return;
}

void CTreeViewPage::OnCancel()
{
	return;
}