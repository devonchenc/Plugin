#include "stdafx.h"
#include "PIMultiDocTemplate.h"

IMPLEMENT_DYNAMIC(CPIMultiDocTemplate, CMultiDocTemplate)

CPIMultiDocTemplate::CPIMultiDocTemplate(UINT nIDResource, CRuntimeClass* pDocClass, CRuntimeClass* pFrameClass, CRuntimeClass* pViewClass)
	: CMultiDocTemplate(nIDResource, pDocClass, pFrameClass, pViewClass)
{

}

CPIMultiDocTemplate::~CPIMultiDocTemplate()
{

}

#ifdef _DEBUG
void CPIMultiDocTemplate::AssertValid() const
{
	CMultiDocTemplate::AssertValid();
}

void CPIMultiDocTemplate::Dump(CDumpContext& dc) const
{
	CMultiDocTemplate::Dump(dc);
}
#endif

CDocTemplate::Confidence CPIMultiDocTemplate::MatchDocType(LPCTSTR pszPathName, CDocument*& rpDocMatch)
{
	ASSERT(pszPathName != NULL);
	rpDocMatch = NULL;

	// go through all documents
	POSITION pos = GetFirstDocPosition();
	while (pos != NULL)
	{
		CDocument* pDoc = GetNextDoc(pos);
		if (pDoc->GetPathName() == pszPathName)
		{
			// already open
			rpDocMatch = pDoc;
			return yesAlreadyOpen;
		}
	}

	// see if it matches either suffix
	CString strFilterExt;
	if (GetDocString(strFilterExt, CDocTemplate::filterExt) && !strFilterExt.IsEmpty())
	{
		// see if extension matches
		ASSERT(strFilterExt[0] == '.');
		int nDot = CString(pszPathName).ReverseFind('.');
		if (nDot < 0)
		{
			// unknown document type
			return yesAttemptForeign;
		}
		LPCTSTR pszDot = nDot < 0 ? NULL : pszPathName + nDot;

		int nSemi = strFilterExt.Find(';');
		if (nSemi != -1)
		{
			while (nSemi != -1)
			{
				CString strExt = strFilterExt.Left(nSemi);
				if (lstrcmpi(pszDot, strExt) == 0)
				{
					return yesAttemptNative; // extension matches
				}
				else
				{
					strFilterExt = strFilterExt.Mid(nSemi + 1);
					nSemi = strFilterExt.Find(';');
				}
			}
			// compare the last one
			if (lstrcmpi(pszDot, strFilterExt) == 0)
			{
				return yesAttemptNative; // extension matches
			}
		}
		else
		{
			// string contains a single extension
			if (lstrcmpi(pszDot, strFilterExt) == 0)
			{
				return yesAttemptNative;  // extension matches
			}
		}
	}
	
	// unknown document type
	return yesAttemptForeign;
}