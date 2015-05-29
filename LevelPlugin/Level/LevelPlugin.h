#pragma once

class CLevelPlugin : public CPlugin
{
protected:
	CLevelPlugin();

public:
	DECLARE_PLUGIN(CLevelPlugin)

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};