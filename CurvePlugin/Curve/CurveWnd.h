#pragma once

// CCurveWnd

#include <vector>
using namespace std;

#define LINE_PATTERN_SPLINE            0
#define LINE_PATTERN_LINEAR            1
#define CHANNEL_RGB                    0
#define CHANNEL_RED                    1
#define CHANNEL_GREEN                  2
#define CHANNEL_BLUE                   3

struct peg
{
	CPoint*                           m_pt;               //坐标点
	double                       m_dRatioX;               //相对Rect位置X
	double                       m_dRatioY;               //相对Rext位置Y
};

typedef CList<peg, peg>               PEGS;               //存放peg的数据结构
typedef vector<int>              TRANSFORM;               //存放灰度变换的向量
typedef vector<double>              VECTOR;               //样条插值函数与追赶法的参数：向量

class CCurveWnd : public CWnd
{
	DECLARE_DYNAMIC(CCurveWnd)

public:
	CCurveWnd();
	virtual ~CCurveWnd();

	// 取得当前图像
	CImage* GetCurImage();
	
	// 通过计算，取得直方图
	BOOL GetHistogram();

	// 看peg是否存在
	BOOL PegIsExist(CPoint& point);

	// 增加新的peg
	POSITION AddNewPeg(CPoint& point);

	// 改变某个peg
	BOOL RePlacePeg(CPoint& point);

	// 看样子用户想增加一个peg
	BOOL UserWillDrag(CPoint& point);

	// 样条插值函数
	void SplineFunc();
	void PieceWiseFunc();                                 //分段线性函数
	BOOL CursorIsInPaintRect(CPoint& point);              //光标在绘图区域之内
	CPoint TransPositionToDialog(CPoint& point);          //转换到对话框默认的坐标系(左上角是零点)
	CPoint TransPositionToRectangle(CPoint& point);       //转换到以Rect左下角为原点的坐标系
	BOOL GrayTransform();                                 //根据曲线作灰度变换
	void Threshold();                                     //对超出[0,255]的灰度进行处理
	int* PreTransform();                                  //对m_pnTransform预操作
	void InverseTransform();                              //图像反转
	void SaveCurrentState();                              //保存当前状态
	void ImshowImmediately();                             //立即刷新图像
	void ShowGrayTransform();                             //插值并显示灰度变换曲线

	void ReShapeButtons(int nItemID, int nButtonPosX1, int nButtonPosX2, double Space, int Number);
	void ReshapePegs();
	void ReshapeTransform();

public:
	//按钮
	CComboBox*                         m_pComboBoxRGB;    //RGB通道:4
	CButton*                            m_pBnGroupBox;    //Group Box
	CMFCButton*                      m_pBnChooseCurve;    //曲线模式
	CMFCButton*                       m_pBnChooseLine;    //直线模式
	CMFCButton*                          m_pBnInverse;    //反转
	CMFCButton*                            m_pBnReset;    //重置
	CMFCButton*                      m_pBnImportCurve;    //导入曲线
	CMFCButton*                      m_pBnExportCurve;    //导出曲线
	CMFCButton*                               m_pBnOK;    //确定
	CMFCButton*                           m_pBnCancel;    //取消
	CEdit*                               m_pXPosition;    //X坐标
	CEdit*                               m_pYPosition;    //Y坐标
	CStatic*                                m_pXlabel;    //X标签
	CStatic*                                m_pYlabel;    //Y标签

	//数据
	double*                           m_pfHistogram[4];    //直方图:4*256
	TRANSFORM                        m_V4Transform[4];    //灰度变换:4*m_nWidth
	PEGS                               m_pPegsList[4];    //图像选的点:4*m_nWidth
	CRect*                             m_pOnPaintRect;    //绘图区域
	BYTE*                                   m_dataSrc;    //图像源数据
	BYTE*                                   m_curData;    //当前图像数据

	int                            m_nChannelSelected;    //当前通道
	BOOL                              m_bUserWillDrag;    //新增peg
	int                                  m_nRectWidth;    //绘图区宽度
	int                                 m_nRectHeight;    //绘图区高度
	double                               m_fWidthRatio;    //宽度缩放比
	double                              m_fHeightRatio;    //高度缩放比
	POSITION                          m_posCurrentPeg;    //当前拽动的peg
	int                                m_nLinePattern;    //线型
	BOOL                                   m_bInverse;    //反转

	//图像信息
	BYTE*                                     m_pBits;    //图像指针
	long                                m_nImageWidth;    //图像宽度
	long                               m_nImageHeight;    //图像高度
	int                                    m_nChannel;    //图像通道
	DWORD                                  m_nlenData;    //图像长度

	//OnSize
	CRect*                                 m_pWndSize;    //当前Wnd大小

	//字体
	CFont*                                 m_pAppFont;    //字体
	int                                   m_nFontSize;    //字体大小
	CString                                m_FontName;    //字体名

	//*** CImage ***//
	CImage*                                m_pImage;    //CImage指针

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg void OnChooseCurve();
	afx_msg void OnChooseLine();
	afx_msg void OnComboboxRgb();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnIdbCurvewndReset();
	afx_msg void OnIdbCurvewndInverse();
	afx_msg void OnIdbImportCurve();
	afx_msg void OnIdbExportCurve();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnIdbCurveOk();
	afx_msg void OnIdbCurveCancel();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

private://新增的私有函数
	VECTOR Spline(VECTOR& Node, VECTOR& Value, VECTOR& x0, double c1, double c2, int CASE = 2);
	void Chase(VECTOR& a, VECTOR& b, VECTOR& c, VECTOR& f, VECTOR& x, int n);
	COLORREF ColorSetting(int Channel, int Strength, int WhereUse = 0);
};