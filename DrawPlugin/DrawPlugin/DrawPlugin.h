// DrawPlugin.h : DrawPlugin DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号

#include <map>
#include "Shape.h"

// CDrawPluginApp
// 有关此类实现的信息，请参阅 DrawPlugin.cpp
//

class CDrawPluginApp : public CWinApp
{
public:
	CDrawPluginApp();

public:
	CShape::shape GetCurrentShapeFlag();

	void SetCurrentShapeFlag(CShape::shape shpFlag);

	CShape* GetCurrentShape(CWnd* pWnd);

	// set current shape
	CShape* NewShape(CWnd* pWnd);

	void ReleaseShape();

	void ChangeMouseListenState();

public:
	// plugin index
	int m_nPluginIndex;

	// shape flag
	CShape::shape m_shpFlag;

	std::map<CWnd*, CShape*> m_ShapeMap;

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
	afx_msg void OnDrawLine();
	afx_msg void OnUpdateDrawLine(CCmdUI *pCmdUI);
	afx_msg void OnDrawStroke();
	afx_msg void OnUpdateDrawStroke(CCmdUI *pCmdUI);
	afx_msg void OnDrawRectangle();
	afx_msg void OnUpdateDrawRectangle(CCmdUI *pCmdUI);
	afx_msg void OnDrawEllipse();
	afx_msg void OnUpdateDrawEllipse(CCmdUI *pCmdUI);
};
