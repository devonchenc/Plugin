// Image2Plugin.h : Image2Plugin DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImage2PluginApp
// �йش���ʵ�ֵ���Ϣ������� Image2Plugin.cpp
//

class CImage2PluginApp : public CWinApp
{
public:
	CImage2PluginApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
