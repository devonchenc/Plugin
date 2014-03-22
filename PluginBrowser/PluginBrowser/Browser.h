#pragma once

class CBrowserPlugin : public CPlugin
{
protected:
	CBrowserPlugin();

public:
	static CBrowserPlugin* Instance() { static CBrowserPlugin _instance; return &_instance; }

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};