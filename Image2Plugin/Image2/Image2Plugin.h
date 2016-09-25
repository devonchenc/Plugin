#pragma once

class CImage2Plugin : public CPlugin
{
	DECLARE_PLUGIN(CImage2Plugin)

private:
	CImage2Plugin(){}

public:
	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};