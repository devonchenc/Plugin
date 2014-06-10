#pragma once

class CLevelPlugin : public CPlugin
{
protected:
	CLevelPlugin();

public:
	static CLevelPlugin* Instance() { static CLevelPlugin _instance; return &_instance; }

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};