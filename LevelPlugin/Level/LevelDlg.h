#pragma once

#include "LevelWnd.h"

// CLevelDlg 对话框

class CLevelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CLevelDlg)

public:
	CLevelDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLevelDlg();

private:
	CLevelWnd m_wndLevel;

public:
// 对话框数据
	enum { IDD = IDD_LEVEL_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
