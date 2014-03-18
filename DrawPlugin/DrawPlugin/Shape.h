#pragma once

class CShape
{
public:
	virtual BOOL Draw(CDC* pDC)=0;
	virtual BOOL Init(CPoint point)=0;
	virtual BOOL UpdatePoint(CPoint point)=0;

public:
	enum shape{
		NONE			= 0,
		LINE			= 1,
		STROKE			= 2,
		RECTANGLE		= 3,
		ELLIPSE			= 4
	};
};

class CLine : public CShape
{
public:
	CPoint m_ptFirst;
	CPoint m_ptSecond;

public:
	virtual BOOL Draw(CDC* pDC);
	virtual BOOL Init(CPoint point);
	virtual BOOL UpdatePoint(CPoint point);
};

class CStroke : public CShape
{
public:
	CStroke();

	// Attributes
public:
	CArray<CPoint,CPoint> m_pointArray;

	// Operations
public:
	virtual BOOL Draw(CDC* pDC);
	virtual BOOL Init(CPoint point);
	virtual BOOL UpdatePoint(CPoint point);
};

class CRectAngle : public CShape
{
public:
	CPoint m_ptFirst;
	CPoint m_ptSecond;

public:
	virtual BOOL Draw(CDC* pDC);
	virtual BOOL Init(CPoint point);
	virtual BOOL UpdatePoint(CPoint point);
};

class CEllipse : public CShape
{
public:
	CPoint m_ptFirst;
	CPoint m_ptSecond;

public:
	virtual BOOL Draw(CDC* pDC);
	virtual BOOL Init(CPoint point);
	virtual BOOL UpdatePoint(CPoint point);
};

