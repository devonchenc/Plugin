#pragma once

#include "PluginDefine.h"

class PLUGIN_EXT_CLASS CPIDocManager : public CDocManager
{
	DECLARE_DYNAMIC(CPIDocManager)

public:
	CPIDocManager(void);
	virtual ~CPIDocManager(void);

public:
	// remove plugin document template
	void RemovePluginDocTemplate();

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	virtual void OnFileNew();
};

