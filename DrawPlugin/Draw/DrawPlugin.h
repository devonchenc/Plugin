#pragma once

class CDrawPlugin : public CPlugin
{
protected:
	CDrawPlugin();

public:
	DECLARE_PLUGIN(CDrawPlugin)

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};