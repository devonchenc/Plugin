#pragma once

class CImageProcessingPlugin : public CPlugin
{
	DECLARE_PLUGIN(CImageProcessingPlugin)

private:
	CImageProcessingPlugin(){}

public:
	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);
};