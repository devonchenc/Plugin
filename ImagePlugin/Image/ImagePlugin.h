#pragma once

class CImagePlugin : public CPlugin
{
protected:
	CImagePlugin();

public:
	static CImagePlugin* Instance() { static CImagePlugin _instance; return &_instance; }

	virtual void Init(CWinApp* pApp, int nPluginIndex);

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};