#pragma once

class CImageProcessingPlugin : public CPlugin
{
protected:
	CImageProcessingPlugin();

public:
	DECLARE_PLUGIN(CImageProcessingPlugin)

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};