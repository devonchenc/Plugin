#pragma once

class CImageProcessingPlugin : public CPlugin
{
protected:
	CImageProcessingPlugin();

public:
	static CImageProcessingPlugin* Instance() { static CImageProcessingPlugin _instance; return &_instance; }

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};