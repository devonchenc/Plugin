#pragma once

#include "CurveWnd.h"

// CCurveDlg 对话框

class CCurveDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCurveDlg)

public:
	CCurveDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCurveDlg();

private:
	CCurveWnd m_wndCurve;

public:
// 对话框数据
	enum { IDD = IDD_CURVE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL OnInitDialog();
};
