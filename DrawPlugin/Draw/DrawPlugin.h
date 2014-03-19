#pragma once

class CDrawPlugin : public CPlugin
{
protected:
	CDrawPlugin();

public:
	static CDrawPlugin* Instance() { static CDrawPlugin _instance; return &_instance; }

	virtual void Init(CWinApp* pApp, int nPluginIndex);

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};