#pragma once
#include "afxcmn.h"


// CListViewPage 对话框

class CListViewPage : public CDialogEx
{
	DECLARE_DYNAMIC(CListViewPage)

public:
	CListViewPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CListViewPage();

public:
	void DisplayPluginInfo(const PluginInfoArray& array);

public:
// 对话框数据
	enum { IDD = IDD_LIST_VIEW_PAGE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CListCtrl m_ctrlPluginList;
	virtual BOOL OnInitDialog();
	afx_msg void OnNMClickPluginList(NMHDR* pNMHDR, LRESULT* pResult);
	virtual void OnOK();
	virtual void OnCancel();
};
