// Image.h : Image DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CImageApp
// �йش���ʵ�ֵ���Ϣ������� Image.cpp
//

class CImageApp : public CWinApp
{
public:
	CImageApp();

public:
	CPIMultiDocTemplate* GetDocTemplate()	{ return m_pDocTemplate; }

private:
	CPIMultiDocTemplate* m_pDocTemplate;

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	afx_msg void OnPluginImage();
};
