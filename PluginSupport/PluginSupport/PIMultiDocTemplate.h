#pragma once

#include "PluginDefine.h"

class PLUGIN_EXT_CLASS CPIMultiDocTemplate : public CMultiDocTemplate
{
	DECLARE_DYNAMIC(CPIMultiDocTemplate)

public:
	CPIMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass);
	virtual ~CPIMultiDocTemplate();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual Confidence MatchDocType(LPCTSTR lpszPathName, CDocument*& rpDocMatch);
};

