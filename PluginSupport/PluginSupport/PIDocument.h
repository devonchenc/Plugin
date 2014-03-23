#pragma once

#include "PluginDefine.h"

// CPIDocument 文档

class PLUGIN_EXT_CLASS CPIDocument : public CDocument
{
	DECLARE_DYNCREATE(CPIDocument)

public:
	CPIDocument();
	virtual ~CPIDocument();

public:
	CImage* GetImage()	{ return m_pImage;}

protected:
	CImage* m_pImage;

public:
#ifndef _WIN32_WCE
	virtual void Serialize(CArchive& ar);   // 为文档 I/O 重写
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
};
