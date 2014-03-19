#include "StdAfx.h"
#include "Shape.h"

BOOL CLine::Draw(CDC* pDC)
{
	pDC->MoveTo(m_ptFirst);
	pDC->LineTo(m_ptSecond);

	return TRUE;
}

BOOL CLine::Init(CPoint point)
{
	m_ptFirst = m_ptSecond = point;
	return TRUE;
}

BOOL CLine::UpdatePoint(CPoint point)
{
	m_ptSecond = point;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

CStroke::CStroke()
{
	m_pointArray.RemoveAll();
}

BOOL CStroke::Init(CPoint point)
{
	m_pointArray.Add(point);
	m_pointArray.Add(point);
	return TRUE;
}

BOOL CStroke::UpdatePoint(CPoint point)
{
	m_pointArray.Add(point);
	return TRUE;
}

BOOL CStroke::Draw(CDC* pDC)
{
	pDC->MoveTo(m_pointArray[0]);
	for (int i=1; i<m_pointArray.GetSize(); ++i)
	{
		pDC->LineTo(m_pointArray[i]);
	}
	return TRUE;
}

//////////////////////////////////////////////////////////////////////////

BOOL CRectAngle::Draw(CDC* pDC)
{
	CBrush* brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* pOldBrush = pDC->SelectObject(brush);
	pDC->Rectangle(m_ptFirst.x, m_ptFirst.y, m_ptSecond.x, m_ptSecond.y);
	pDC->SelectObject(pOldBrush);

	return TRUE;
}

BOOL CRectAngle::Init(CPoint point)
{
	m_ptFirst = m_ptSecond = point;
	return TRUE;
}

BOOL CRectAngle::UpdatePoint(CPoint point)
{
	m_ptSecond = point;
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////

BOOL CEllipse::Init(CPoint point)
{
	m_ptFirst = m_ptSecond = point;
	return TRUE;
}

BOOL CEllipse::UpdatePoint(CPoint point)
{
	m_ptSecond = point;
	return TRUE;
}

BOOL CEllipse::Draw(CDC* pDC)
{
	CBrush* brush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));
	CBrush* pOldBrush = pDC->SelectObject(brush);
	pDC->Ellipse(m_ptFirst.x, m_ptFirst.y, m_ptSecond.x, m_ptSecond.y);
	pDC->SelectObject(pOldBrush);

	return TRUE;
}
