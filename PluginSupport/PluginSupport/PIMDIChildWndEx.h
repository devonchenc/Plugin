#pragma once

#include "PluginDefine.h"

// CPIMDIChildWndEx

class PLUGIN_EXT_CLASS CPIMDIChildWndEx : public CMDIChildWndEx
{
	DECLARE_DYNCREATE(CPIMDIChildWndEx)

public:
	CPIMDIChildWndEx();
	virtual ~CPIMDIChildWndEx();

protected:
	DECLARE_MESSAGE_MAP()
};
