#pragma once

class CImagePlugin : public CPlugin
{
	DECLARE_PLUGIN(CImagePlugin)

private:
	CImagePlugin(){}

public:
	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};