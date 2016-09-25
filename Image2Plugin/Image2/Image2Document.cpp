// ImageDocument.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Image2.h"
#include "Image2Document.h"


// CImageDocument

IMPLEMENT_DYNCREATE(CImage2Document, CPIDocument)

CImage2Document::CImage2Document()
{

}

BOOL CImage2Document::OnNewDocument()
{
	if (!CPIDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CImage2Document::~CImage2Document()
{

}

BEGIN_MESSAGE_MAP(CImage2Document, CPIDocument)
END_MESSAGE_MAP()

// CImageDocument ���

#ifdef _DEBUG
void CImage2Document::AssertValid() const
{
	CPIDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CImage2Document::Dump(CDumpContext& dc) const
{
	CPIDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CImageDocument ���л�

void CImage2Document::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}
#endif

// CImageDocument ����

BOOL CImage2Document::OnOpenDocument(LPCTSTR lpszPathName)
{
	if (!CPIDocument::OnOpenDocument(lpszPathName))
		return FALSE;

	// TODO:  �ڴ������ר�õĴ�������
	CString strPathName = lpszPathName;
	int nPos = strPathName.ReverseFind('.');
	// ȡ�ú�׺��
	CString strExt = strPathName.Right(strPathName.GetLength() - nPos - 1);
	strExt.MakeLower();

	int nType = FindType(strExt);
	if (nType != IMAGE_FORMAT_UNKNOWN)
	{
		m_pImage = new CImage;
		m_pImage->Load(lpszPathName);
	}

	return TRUE;
}

int CImage2Document::FindType(const CString strExt)
{
	if (strExt.CompareNoCase(_T("bmp")) == 0)
	{
		return IMAGE_FORMAT_BMP;
	}
	else if (strExt.CompareNoCase(_T("jpg")) == 0 || strExt.CompareNoCase(_T("jpeg")) == 0)
	{
		return IMAGE_FORMAT_JPG;
	}
	else if (strExt.CompareNoCase(_T("gif")) == 0)
	{
		return IMAGE_FORMAT_GIF;
	}
	else if (strExt.CompareNoCase(_T("png")) == 0)
	{
		return IMAGE_FORMAT_PNG;
	}
	else if (strExt.CompareNoCase(_T("tif")) == 0 || strExt.CompareNoCase(_T("tifF")) == 0)
	{
		return IMAGE_FORMAT_TIF;
	}

	return IMAGE_FORMAT_UNKNOWN;
}