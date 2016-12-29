// Image.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "Image2.h"
#include "Image2Document.h"
#include "Image2View.h"
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

BEGIN_MESSAGE_MAP(CImage2App, CWinApp)
	ON_COMMAND(ID_PLUGIN_IMAGE, &CImage2App::OnPluginImage)
END_MESSAGE_MAP()


// CImageApp ����

CImage2App::CImage2App()
{
	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CImageApp ����

CImage2App theApp;


// CImageApp ��ʼ��

BOOL CImage2App::InitInstance()
{
	CWinApp::InitInstance();

	// ע��Ӧ�ó�����ĵ�ģ�塣�ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	m_pDocTemplate = new CPIMultiDocTemplate(IDR_IMAGE,
		RUNTIME_CLASS(CImage2Document),
		RUNTIME_CLASS(CPIMDIChildWndEx), // �Զ��� MDI �ӿ��
		RUNTIME_CLASS(CImage2View));
	if (!m_pDocTemplate)
		return FALSE;

	return TRUE;
}


void CImage2App::OnPluginImage()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	AfxMessageBox(_T("OnPluginImage2")); 
	CWnd* pMainWnd = PIGetMainWnd();
	pMainWnd->PostMessage(WMU_FILE_NEW, m_nPluginId, m_nDocTemplateId);	// good
}
