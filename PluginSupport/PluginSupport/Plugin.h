#pragma once

#include "PluginDefine.h"
#include "PIMultiDocTemplate.h"

class PLUGIN_EXT_CLASS CPlugin
{
protected:
	CPlugin(){}

private:
	CPlugin(const CPlugin&);
	CPlugin& operator=(const CPlugin&);

public:
	virtual void Init(CWinApp* pApp, int nPluginIndex){}

	virtual void Release(){}

	virtual void Query(CPluginInfo& plugininfo){}

	virtual void Execute(UINT nCommandID, CCmdUI* pCmdUI)
	{
		AFX_MANAGE_STATE(AfxGetStaticModuleState());

		CWinApp* pApp = AfxGetApp();
		if (pCmdUI == NULL)
		{
			pApp->OnCmdMsg(nCommandID, CN_COMMAND, NULL, NULL);
		}
		else
		{
			pApp->OnCmdMsg(nCommandID, CN_UPDATE_COMMAND_UI, pCmdUI, NULL);
		}
	}

	virtual void Draw(CDC* pDC){}

	virtual int GetDocTemplateCount()	{ return 0; }

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex)	{ return NULL; }

// member variables
protected:
};