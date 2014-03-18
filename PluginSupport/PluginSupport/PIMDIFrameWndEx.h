#pragma once

#include "PluginDefine.h"

// CPIMDIFrameWndEx 框架

class PLUGIN_EXT_CLASS CPIMDIFrameWndEx : public CMDIFrameWndEx
{
	DECLARE_DYNCREATE(CPIMDIFrameWndEx)
protected:
	CPIMDIFrameWndEx();           // 动态创建所使用的受保护的构造函数
	virtual ~CPIMDIFrameWndEx();

protected:

protected:
	afx_msg LRESULT OnUIEnableEvent(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUICheckEvent(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPluginCommand(UINT nID);
	afx_msg void OnUpdatePluginCommand(CCmdUI* pCmdUI);
	afx_msg LRESULT OnGetActiveDocument(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnGetActiveView(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
};


