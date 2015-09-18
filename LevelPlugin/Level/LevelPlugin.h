#pragma once

class CLevelPlugin : public CPlugin
{
	DECLARE_PLUGIN(CLevelPlugin)

private:
	CLevelPlugin(){}

public:
	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);
};