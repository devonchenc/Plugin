// PluginBrowserDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginBrowser.h"
#include "PluginBrowserDlg.h"
#include "afxdialogex.h"


// CPluginBrowserDlg 对话框

IMPLEMENT_DYNAMIC(CPluginBrowserDlg, CDialogEx)

CPluginBrowserDlg::CPluginBrowserDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPluginBrowserDlg::IDD, pParent)
{
	m_pActiveArray = &m_PluginInfoArray;
}

CPluginBrowserDlg::~CPluginBrowserDlg()
{
	for (int i = 0; i < m_PluginInfoArray.GetSize(); i++)
	{
		CPluginInfo* pPluginInfo = m_PluginInfoArray.GetAt(i);
		delete pPluginInfo;
	}
	m_PluginInfoArray.RemoveAll();
}

void CPluginBrowserDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_TabCtrl);
}


BEGIN_MESSAGE_MAP(CPluginBrowserDlg, CDialogEx)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CPluginBrowserDlg::OnTcnSelchangeTab1)
	ON_EN_CHANGE(IDC_SEARCH_EDIT, &CPluginBrowserDlg::OnEnChangeSearchEdit)
	ON_BN_CLICKED(IDC_CLEAR, &CPluginBrowserDlg::OnBnClickedClear)
END_MESSAGE_MAP()


// CPluginBrowserDlg 消息处理程序

BOOL CPluginBrowserDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// add two pages to TabControl
	CString str;
	str.LoadString(IDS_STRING_TAB_TEXT1);
	m_TabCtrl.InsertItem(0, str);
	str.LoadString(IDS_STRING_TAB_TEXT2);
	m_TabCtrl.InsertItem(1, str);

	// 创建两个对话框
	m_PageList.Create(IDD_LIST_VIEW_PAGE, &m_TabCtrl);
	m_PageTree.Create(IDD_TREE_VIEW_PAGE, &m_TabCtrl);

	CRect rect;
	m_TabCtrl.GetClientRect(&rect);
	rect.top += 20;
	// 将子页面移动到指定的位置
	m_PageList.MoveWindow(rect);
	m_PageTree.MoveWindow(rect);
	// 显示子页面
	m_PageList.ShowWindow(SW_SHOW);
	m_PageTree.ShowWindow(SW_HIDE);

	// search plugin
	SearchPlugin();

	// display plugin info
	m_PageList.DisplayPluginInfo(m_PluginInfoArray);
	m_PageTree.DisplayPluginInfo(m_PluginInfoArray);

	// display plugin num
	DisplayPluginNum();
	GetDlgItem(IDC_SEARCH_EDIT)->SetFocus();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

// search plugin
void CPluginBrowserDlg::SearchPlugin()
{
	CString strPath = GetApplicationPath();
	strPath += _T("*.dll");

	CFileFind find;
	BOOL bIsFind = find.FindFile(strPath);
	while (bIsFind)
	{
		bIsFind = find.FindNextFile();

		if (find.IsDots())
			continue;
		else if (find.IsDirectory())
			continue;
		else
		{
			CString strFileName = find.GetFileName();
			// load dll
			LoadDLL(strFileName);
		}
	}
}

CString CPluginBrowserDlg::GetApplicationPath()
{
	TCHAR filename[_MAX_PATH];
	TCHAR drive[_MAX_DRIVE];
	TCHAR path[_MAX_PATH];

	GetModuleFileName(AfxGetInstanceHandle(), filename, MAX_PATH);
	_wsplitpath_s(filename, drive, _MAX_DRIVE, path, _MAX_PATH, NULL, 0, NULL, 0);

	CString strLocation(drive);
	strLocation += path;

	return strLocation;
}

// load dll
void CPluginBrowserDlg::LoadDLL(CString strFileName)
{
	HINSTANCE hInstance = LoadLibrary(strFileName);
	if (hInstance != NULL)
	{
		GETINSTANCE pfnGetInstance = (GETINSTANCE)GetProcAddress(hInstance, "GetInstance");
		if (pfnGetInstance == NULL)
		{
			FreeLibrary(hInstance);
			return;
		}

		CPlugin* pPlugin = pfnGetInstance();
		if (pPlugin == NULL)
		{
			FreeLibrary(hInstance);
			return;
		}

		CPluginInfo* pInfo = new CPluginInfo;
		pPlugin->Query(*pInfo);
		m_PluginInfoArray.Add(pInfo);

		FreeLibrary(hInstance);
	}
}

void CPluginBrowserDlg::OnTcnSelchangeTab1(NMHDR* pNMHDR, LRESULT* pResult)
{
	switch (m_TabCtrl.GetCurSel())
	{
	case 0:
		m_PageList.ShowWindow(SW_SHOW);
		m_PageTree.ShowWindow(SW_HIDE);
		break;
	case 1:
		m_PageList.ShowWindow(SW_HIDE);
		m_PageTree.ShowWindow(SW_SHOW);
		break;
	default:
		break;
	}

	*pResult = 0;
}

void CPluginBrowserDlg::OnEnChangeSearchEdit()
{
	// clear
	m_FindArray.RemoveAll();

	CString strEdit;
	GetDlgItemText(IDC_SEARCH_EDIT, strEdit);
	strEdit = strEdit.MakeLower();
	// 当文本框中没有输入字符时，将所有插件信息在ListCtrl中输出
	if (strEdit.IsEmpty())
	{
		m_pActiveArray = &m_PluginInfoArray;

		// update ListCtrl
		m_PageList.DisplayPluginInfo(m_PluginInfoArray);

		// update TreeCtrl
		m_PageTree.DisplayPluginInfo(m_PluginInfoArray);

		PrintPluginInfo();

		DisplayPluginNum();

		return;
	}

	// 查找匹配字符串
	for (int i=0; i<m_PluginInfoArray.GetSize(); i++)
	{
		CPluginInfo* pInfo = m_PluginInfoArray.GetAt(i);
		CString strName = pInfo->m_strName;
		strName.MakeLower();
		int nIndex = strName.Find(strEdit);
		if (nIndex > -1)
		{
			m_FindArray.Add(pInfo);
		}
	}

	m_pActiveArray = &m_FindArray;

	// update ListCtrl
	m_PageList.DisplayPluginInfo(m_FindArray);

	// update TreeCtrl
	m_PageTree.DisplayPluginInfo(m_FindArray);

	PrintPluginInfo();

	DisplayPluginNum();
}

void CPluginBrowserDlg::OnBnClickedClear()
{
	SetDlgItemText(IDC_SEARCH_EDIT, _T(""));
	GetDlgItem(IDC_SEARCH_EDIT)->SetFocus();

	m_FindArray.RemoveAll();

	m_pActiveArray = &m_PluginInfoArray;

	PrintPluginInfo();
}

// display plugin info in RichEdit
void CPluginBrowserDlg::PrintPluginInfo(int nIndex)
{
	int nSize = m_pActiveArray->GetSize();
	if (nSize > 0 && nIndex >= 0 && nIndex < nSize)
	{
		CPluginInfo* pInfo = m_pActiveArray->GetAt(nIndex);
		if (pInfo)
		{
			CString strName;
			strName.LoadString(IDS_STRING_PLUGIN_NAME);
			CString strBlurd;
			strBlurd.LoadString(IDS_STRING_PLUGIN_BLURD);
			CString strHelp;
			strHelp.LoadString(IDS_STRING_PLUGIN_HELP);
			CString strAuthor;
			strAuthor.LoadString(IDS_STRING_PLUGIN_AUTHOR);
			CString strCopyright;
			strCopyright.LoadString(IDS_STRING_PLUGIN_COPYRIGHT);
			CString strDate;
			strDate.LoadString(IDS_STRING_PLUGIN_DATE);
			CString strMenuLabel;
			strMenuLabel.LoadString(IDS_STRING_PLUGIN_MENU_LABLE);

			CString strOutput;
			strOutput.Format(_T("%s:\n    %s\n\n%s:\n    %s\n\n%s:\n    %s\n\n%s:\n    %s\n\n%s:\n    %s\n\n%s:\n    %s\n\n%s:\n    %s"),
							strName, pInfo->m_strName, strBlurd, pInfo->m_strBlurb, strHelp, pInfo->m_strHelp,
							strAuthor, pInfo->m_strAuthor, strCopyright, pInfo->m_strCopyRight, strDate, pInfo->m_strDate,
							strMenuLabel, pInfo->m_strMenuLabel);
			SetDlgItemText(IDC_RICHEDIT21, strOutput);
			return;
		}
	}

	// none match
	CString str;
	str.LoadString(IDS_STRING_NO_MATCH);
	SetDlgItemText(IDC_RICHEDIT21, str);
}

// display plugin's number
void CPluginBrowserDlg::DisplayPluginNum()
{
	if (m_pActiveArray->GetSize() > 0)
	{
		CString str;
		str.LoadString(IDS_STRING_PLUGIN_NUM);
		CString strNum;
		strNum.Format(_T("%d %s"), m_pActiveArray->GetSize(), str);
		GetDlgItem(IDC_PLUGIN_NUM)->SetWindowText(strNum);
	}
	else
	{
		CString str;
		str.LoadString(IDS_STRING_PLUGIN_NUM_NONE);
		GetDlgItem(IDC_PLUGIN_NUM)->SetWindowText(str);
	}
}