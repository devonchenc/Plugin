// Image.h : Image DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号


// CImageApp
// 有关此类实现的信息，请参阅 Image.cpp
//

class CImageApp : public CWinApp
{
public:
	CImageApp();

public:
	CPIMultiDocTemplate* GetDocTemplate(int nPluginId, int nDocTemplateId)	{ 
		m_nPluginId = nPluginId;
		m_nDocTemplateId = nDocTemplateId;
		return m_pDocTemplate;
	}

private:
	CPIMultiDocTemplate* m_pDocTemplate;
	int m_nPluginId;
	int m_nDocTemplateId;

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPluginImage();
};
