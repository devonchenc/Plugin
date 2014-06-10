#pragma once
class CCurvePlugin : public CPlugin
{
protected:
	CCurvePlugin(void);

public:
	static CCurvePlugin* Instance() { static CCurvePlugin _instance; return &_instance; }

	virtual void Init();

	virtual void Release();

	virtual void Query(CPluginInfo& plugininfo);

	virtual void Draw(CDC* pDC);

	virtual int GetDocTemplateCount();

	virtual CPIMultiDocTemplate* GetDocTemplate(int nIndex);
};