#pragma once


// CPIDockablePane

class CPIDockablePane : public CDockablePane
{
	DECLARE_DYNAMIC(CPIDockablePane)

public:
	CPIDockablePane(HINSTANCE hInstance, CWnd* pChildWnd);
	virtual ~CPIDockablePane();

private:
	HINSTANCE m_hInstance;
	CWnd* m_pChildWnd;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
};