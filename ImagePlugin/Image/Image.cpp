// Image.cpp : ���� DLL �ĳ�ʼ�����̡�
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
//TODO: ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ������ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CImageApp

BEGIN_MESSAGE_MAP(CImageApp, CWinApp)
	ON_COMMAND(ID_PLUGIN_IMAGE, &CImageApp::OnPluginImage)
END_MESSAGE_MAP()


// CImageApp ����

CImageApp::CImageApp()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CImageApp ����

CImageApp theApp;


// CImageApp ��ʼ��

BOOL CImageApp::InitInstance()
{
	CWinApp::InitInstance();

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	m_pDocTemplate = new CPIMultiDocTemplate(IDR_IMAGE,
		RUNTIME_CLASS(CImageDocument),
		RUNTIME_CLASS(CPIMDIChildWndEx), // �Զ��� MDI �ӿ��
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
