#pragma once

class CBrowserPlugin : public CPlugin
{
	DECLARE_PLUGIN(CBrowserPlugin)

private:
	CBrowserPlugin(){}

public:
	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);
};