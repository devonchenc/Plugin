#pragma once

#include "PluginDefine.h"

// CPIView 视图

class PLUGIN_EXT_CLASS CPIView : public CScrollView
{
	DECLARE_DYNCREATE(CPIView)

protected:
	CPIView();           // 动态创建所使用的受保护的构造函数
	virtual ~CPIView();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void OnDraw(CDC* pDC);      // 重写以绘制该视图
	virtual void OnInitialUpdate();     // 构造后的第一次
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	LRESULT OnPrepareDCEvent(WPARAM wParam, LPARAM lParam);
};


