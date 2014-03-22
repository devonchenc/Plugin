#pragma once

#include "PluginDefine.h"

// CPIToolBar

class PLUGIN_EXT_CLASS CPIToolBar : public CMFCToolBar
{
	DECLARE_DYNAMIC(CPIToolBar)

public:
	CPIToolBar();
	virtual ~CPIToolBar();

public:
	// edit image
	void RemapImage(UINT nCommandID, int nImageIndex);

public:
#ifdef _DEBUG
	virtual void AssertValid() const;
#endif

public:
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	virtual BOOL LoadToolBarExtend(UINT uiResID, UINT nCommandIDIndex, UINT uiColdResID = 0, UINT uiMenuResID = 0,
					BOOL bLocked = FALSE, UINT uiDisabledResID = 0, UINT uiMenuDisabledResID = 0,  UINT uiHotResID = 0);
	virtual BOOL LoadToolBarExtend2(UINT uiToolbarResID, UINT nCommandIDIndex, CMFCToolBarInfo& params, BOOL bLocked = FALSE);

protected:
	DECLARE_MESSAGE_MAP()
};


