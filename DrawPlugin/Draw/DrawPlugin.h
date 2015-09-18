#pragma once

class CDrawPlugin : public CPlugin
{
	DECLARE_PLUGIN(CDrawPlugin)

private:
	CDrawPlugin(){}

public:
	virtual void Init();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);
};