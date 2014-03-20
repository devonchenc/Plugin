#pragma once

class CPluginWrapper;

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
	void SetPluginIndex(int nPluginIndex) { m_nPluginIndex = nPluginIndex; }

	virtual void Init(){}

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

protected:
	int MergeMenu(const CMenu* pMenuAdd, BOOL bTopLevel = FALSE);

	// merge toolbar
	int MergeToolbar(UINT nIDResource);

	// insert toolbar
	int InsertToolbar(UINT nIDResource, CString strText);

private:
	// implement of merge menu
	int MergeMenuImpl(CMenu* pMenuDestination, const CMenu* pMenuAdd, CPluginWrapper* pPluginWrapper, BOOL bTopLevel =FALSE);

// member variables
private:
	int m_nPluginIndex;
};