// Image.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Image.h"
#include "ImageDocument.h"
#include "ImageView.h"
#include <afxMDIChildWndEx.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: 如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CImageApp

BEGIN_MESSAGE_MAP(CImageApp, CWinApp)
	ON_COMMAND(ID_PLUGIN_IMAGE, &CImageApp::OnPluginImage)
END_MESSAGE_MAP()


// CImageApp 构造

CImageApp::CImageApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CImageApp 对象

CImageApp theApp;


// CImageApp 初始化

BOOL CImageApp::InitInstance()
{
	CWinApp::InitInstance();

	// 注册应用程序的文档模板。文档模板
	// 将用作文档、框架窗口和视图之间的连接
	m_pDocTemplate = new CPIMultiDocTemplate(IDR_IMAGE,
		RUNTIME_CLASS(CImageDocument),
		RUNTIME_CLASS(CPIMDIChildWndEx), // 自定义 MDI 子框架
		RUNTIME_CLASS(CImageView));
	if (!m_pDocTemplate)
		return FALSE;

	return TRUE;
}


void CImageApp::OnPluginImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	AfxMessageBox(_T("OnPluginImage"));

	CWinApp* pMainApp = PIGetMainApp();
	pMainApp->PostThreadMessage(ID_FILE_NEW, 0, 0); // failed: Thread = NULL
//	::PostAppMessage()

	// 
	CWnd* pMainWnd = PIGetMainWnd();
//	pMainWnd->SendMessage(WM_CLOSE, 0, 0);	// good
	pMainWnd->PostMessage(WMU_FILE_NEW, m_nPluginId, m_nDocTemplateId);	// good
}
