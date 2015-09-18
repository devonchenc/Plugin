// PIDocument.cpp : 实现文件
//

#include "stdafx.h"
#include "PluginLibrary.h"
#include "PIDocument.h"


// CPIDocument

IMPLEMENT_DYNCREATE(CPIDocument, CDocument)

CPIDocument::CPIDocument()
	: m_pImage(NULL)
{

}

BOOL CPIDocument::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CPIDocument::~CPIDocument()
{
	if (m_pImage)
	{
		delete m_pImage;
		m_pImage = NULL;
	}
}

BEGIN_MESSAGE_MAP(CPIDocument, CDocument)
END_MESSAGE_MAP()

// CPIDocument 诊断

#ifdef _DEBUG
void CPIDocument::AssertValid() const
{
	CDocument::AssertValid();
}

#ifndef _WIN32_WCE
void CPIDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif
#endif //_DEBUG

#ifndef _WIN32_WCE
// CPIDocument 序列化

void CPIDocument::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}
#endif

// CPIDocument 命令
