// Process.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "Process.h"

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

// CProcessApp

BEGIN_MESSAGE_MAP(CProcessApp, CWinApp)
	ON_COMMAND(ID_IMAGE_INVERSE, &CProcessApp::OnImageInverse)
	ON_UPDATE_COMMAND_UI(ID_IMAGE_INVERSE, &CProcessApp::OnUpdateImageInverse)
END_MESSAGE_MAP()


// CProcessApp 构造

CProcessApp::CProcessApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CProcessApp 对象

CProcessApp theApp;


// CProcessApp 初始化

BOOL CProcessApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

UINT InverseThread(LPVOID pParam)
{
	LANGID id = PIGetThreadUILanguage();
	SetThreadUILanguage(id);

	CString str;
	str.LoadString(ID_IMAGE_INVERSE);
//	PIProgressInit(PI_PROGRESS_DLG, str);
	PIProgressInit(PI_PROGRESS_BAR, str);

	CImage* pImage = (CImage*)pParam;

	int nWidth = pImage->GetWidth();
	int nHeight = pImage->GetHeight();
	BYTE* pImageData = (BYTE*)pImage->GetBits();
	int nPitch = pImage->GetPitch();
	int nBitCount = pImage->GetBPP() / 8;

	for (int j=0; j<nHeight; j++)
	{
		for (int i=0; i<nWidth; i++)
		{
			int nPixelIndex = j * nPitch + i * nBitCount;
			BYTE* pPixel = pImageData + j * nPitch + i * nBitCount;
			*(pPixel) = 255 - *(pPixel);
			*(pPixel + 1) = 255 - *(pPixel + 1);
			*(pPixel + 2) = 255 - *(pPixel + 2);
		}

		LRESULT lResult = PIProgressPercent(j * 100 / nHeight);
		if (!lResult)
		{
			// exit thread
			return 0;
		}
	}

	PIProgressDone();

	// refresh view
	PIGetActiveView()->Invalidate(FALSE);

	return 0;
}

void CProcessApp::OnImageInverse()
{
	CDocument* pDoc = PIGetActiveDocument();
	if (pDoc == NULL)
		return;
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CPIDocument)))
		return;

	CImage* pImage = ((CPIDocument*)pDoc)->GetImage();
	if (pImage == NULL)
		return;

	AfxBeginThread(InverseThread, pImage);
}

void CProcessApp::OnUpdateImageInverse(CCmdUI* pCmdUI)
{
	CDocument* pDoc = PIGetActiveDocument();
	PIEnableUI(pCmdUI, pDoc != NULL);
}