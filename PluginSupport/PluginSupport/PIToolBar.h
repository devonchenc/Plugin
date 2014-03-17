#pragma once

#include "PluginImpl.h"

// CPIToolBar

class PLUGIN_EXT_CLASS CPIToolBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(CPIToolBar)

public:
	CPIToolBar();
	virtual ~CPIToolBar();

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

public:
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;

protected:
	DECLARE_MESSAGE_MAP()
};


