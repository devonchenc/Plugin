#pragma once

class CLevelPlugin : public CPlugin
{
private:
	CLevelPlugin(){}

public:
	DECLARE_PLUGIN(CLevelPlugin)

	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);
};