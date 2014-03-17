#include "stdafx.h"
#include "PIDocManager.h"

IMPLEMENT_DYNAMIC(CPIDocManager, CDocManager)

CPIDocManager::CPIDocManager(void)
{

}

CPIDocManager::~CPIDocManager(void)
{

}

#ifdef _DEBUG
void CPIDocManager::AssertValid() const
{
	CDocManager::AssertValid();
}

void CPIDocManager::Dump(CDumpContext& dc) const
{
	CDocManager::Dump(dc);
}
#endif

void CPIDocManager::OnFileNew()
{
	if (m_templateList.IsEmpty())
	{
		TRACE(traceAppMsg, 0, "Error: no document templates registered with CWinApp.\n");
		AfxMessageBox(AFX_IDP_FAILED_TO_CREATE_DOC);
		return;
	}

	CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetHead();
	if (m_templateList.GetCount() > 1)
	{
		// use default template instead
	/*	// more than one document template to choose from
		// bring up dialog prompting user
		CNewTypeDlg dlg(&m_templateList);
		INT_PTR nID = dlg.DoModal();
		if (nID == IDOK)
			pTemplate = dlg.m_pSelectedTemplate;
		else
			return;     // none - cancel operation
	*/
	}

	ASSERT(pTemplate != NULL);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	pTemplate->OpenDocumentFile(NULL);
	// if returns NULL, the user has already been alerted
}

// remove plugin document template
void CPIDocManager::RemovePluginDocTemplate()
{
	while (m_templateList.GetSize() > 1)
	{
		m_templateList.RemoveTail();
	}
}
