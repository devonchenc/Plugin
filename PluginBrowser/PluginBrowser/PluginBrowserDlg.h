#pragma once

#include "afxcmn.h"
#include "ListViewPage.h"
#include "TreeViewPage.h"

// CPluginBrowserDlg 对话框

typedef CPlugin* (*GETINSTANCE)();

class CPluginBrowserDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPluginBrowserDlg)

public:
	CPluginBrowserDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPluginBrowserDlg();

public:
	// display plugin's number
	void DisplayPluginNum();

	// display plugin info in RichEdit
	void PrintPluginInfo(int nIndex = 0);

private:
	// search plugin
	void SearchPlugin();

	CString GetApplicationPath();

	// load dll
	void LoadDLL(CString strFileName);

private:
	// 存储插件信息的指针数组
	PluginInfoArray m_PluginInfoArray;

	// 存储查找到的包含文本编辑框中输入的文本的插件信息的指针数组
	PluginInfoArray m_FindArray;

	// current active array
	PluginInfoArray* m_pActiveArray;

public:
// 对话框数据
	enum { IDD = IDD_PLUGIN_BROWSER_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CTabCtrl m_TabCtrl;
	CListViewPage m_PageList;
	CTreeViewPage m_PageTree;
	afx_msg void OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnEnChangeSearchEdit();
	afx_msg void OnBnClickedClear();
};
