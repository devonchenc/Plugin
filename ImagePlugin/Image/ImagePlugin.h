#pragma once

class CImagePlugin : public CPlugin
{
protected:
	CImagePlugin();

public:
	DECLARE_PLUGIN(CImagePlugin)

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};