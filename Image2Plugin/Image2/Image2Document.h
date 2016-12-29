#pragma once

// CImageDocument �ĵ�

enum ENUM_IMAGE_FORMATS
{
	IMAGE_FORMAT_UNKNOWN = 0,
	IMAGE_FORMAT_BMP = 1,
	IMAGE_FORMAT_GIF = 2,
	IMAGE_FORMAT_JPG = 3,
	IMAGE_FORMAT_PNG = 4,
	IMAGE_FORMAT_TIF = 5
};

class CImage2Document : public CPIDocument
{
	DECLARE_DYNCREATE(CImage2Document)

public:
	CImage2Document();
	virtual ~CImage2Document();

public:
	// find supported image type
	int FindType(const CString strExt);

#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // Ϊ�ĵ� I/O ��д
#endif
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual BOOL OnNewDocument();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnOpenDocument(LPCTSTR lpszPathName);
};
