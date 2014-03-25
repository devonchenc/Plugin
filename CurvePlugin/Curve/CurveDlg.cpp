// CurveDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "Curve.h"
#include "CurveDlg.h"
#include "afxdialogex.h"


// CCurveDlg 对话框

IMPLEMENT_DYNAMIC(CCurveDlg, CDialogEx)

CCurveDlg::CCurveDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCurveDlg::IDD, pParent)
{

}

CCurveDlg::~CCurveDlg()
{
}

void CCurveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCurveDlg, CDialogEx)
END_MESSAGE_MAP()


// CCurveDlg 消息处理程序
