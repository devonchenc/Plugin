#pragma once
#include "ItemInfo.h"
#include "LevelSlider.h"
#include <vector>
using std::vector;

// CLevelWnd

class CLevelWnd : public CWnd
{
	DECLARE_DYNAMIC(CLevelWnd)

public:
	//构造函数
	CLevelWnd();
	//析构函数
	virtual ~CLevelWnd();
protected:
	DECLARE_MESSAGE_MAP()
public:
	/************************************************************************/
	/* 消息响应型函数                                                                                 */
	/************************************************************************/
	//通道选择Combobox
	afx_msg void OnSelchangeComboChannel();
	//重置Button
	afx_msg void OnButtonReset();
	//XLeft文本编辑
	afx_msg void OnEditXLeft();
	//XRight文本编辑
	afx_msg void OnEditXRight();
	//Proportion文本编辑
	afx_msg void OnEditProportion();
	//YTop文本编辑
	afx_msg void OnEditYTop();
	//YBottom文本编辑
	afx_msg void OnEditYBottom();
	//初始化
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//窗口重绘
	afx_msg void OnPaint();
	//背景擦除
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//数据交换
	virtual void DoDataExchange(CDataExchange* pDX);
	//响应Dlg的WM_SIZE
	void ReSize(const UINT& nType, const CRect& Rect);
	//截断回车消息
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	/************************************************************************/
	/*数据传递型函数                                                                                  */
	/************************************************************************/
	//获取图片
	CImage* GetImage();
	//初始化灰度直方图
	void InitGrayValues();
	//初始化灰度映射表
	void InitGrayMap();
	//初始化几何&灰度映射范围
	void InitMapRect(const CRect& rect);
	//绘制直方图
	void DrawHistogram(CDC& memDC);
	//绘制灰度条
	void DrawBar(CDC& memDC);
	//设定直方图颜色
	COLORREF DrawColor();
	//创建控件
	void NewItems();
	//灰度映射
	void GrayMapping();
	//设置控件位置
	BOOL SetItemPosition(const CRect& newRect, const UINT& nIndex);
	//更新图片
	BOOL UpdateImage();
	//拷贝图片
	BOOL ImageCopy(const CImage& m_Image, CImage& m_CopyImage);
	//判断输入
	BOOL CheckNum(CString );

	//坐标转灰度
	//Wnd调用坐标转灰度
	void CoordinateToGray(UINT nID, LONG center);
	//xLeft坐标转xLeft灰度值
	void XLeftCoordinateToGray();
	//xRight坐标转xRight灰度值
	void XRightCoordinateToGray();
	//拐点x坐标转拐点比例
	void XInflectionToProportion();
	//yTop坐标转yTop灰度值
	void YTopCoordinateToGray();
	//yBottom坐标转yBottom灰度值
	void YBottomCoordinateToGray();

	//灰度转坐标
	//xLeft灰度值转xLeft坐标
	void XLeftGrayToCoordinate();
	//xRight灰度值转xRight坐标
	void XRightGrayToCoordinate();
	//拐点比例转拐点x坐标
	void ProportionToXInflection();
	//yTop灰度值转yTop坐标
	void YTopGrayToCoordinate();
	//yBottom灰度值转yBottom坐标
	void YBottomGrayToCoordinate();

	//更新坐标
	//更新xLeft坐标
	void XLeftChangeCoordinate();
	//更新xRight坐标
	void XRightChangeCoordinate();
	//更新拐点坐标
	void XInflectionChangeCoordinate();
	//更新yTop坐标
	void YTopChangeCoordinate();
	//更新yBottom坐标
	void YBottomChangeCoordinate();
	
	public:
		/************************************************************************/
		/* 存储型数据                                                                                        */
		/************************************************************************/
	//矩阵最大值
	float m_ftMaxMatrix[4];
	//矩阵最小值
	float m_ftMinMatrix[4];
	//转换比例(ft)
	float m_ftTranslateProportion[4];
	//Wnd坐标
	CRect m_rect;
	//直方图坐标
	CRect m_HistogramRect;
	//灰度直方图绘制参数
	float m_arrGrayValues[4][256];
	//灰度映射数组
	int m_arrLevelMap[4][256];
	//图像指针
	CImage* m_pImage;
	//原图备份
	CImage m_CopyImage;
	//控件字体
	CFont m_CtrlFont;
	//元素集合
	vector<CItemInfo> m_Items;
	//几何映射区间坐标
	CRect m_arrCoordinateRact[4];
	//色阶映射区间
	CRect m_arrLevelMapRect[4];
	//几何X拐点坐标
	int m_arrXInflection[4];
	//色阶映射比例
	float m_ftArrProportion[4];
	//转换比例(str)
	CString m_strArrProportion[4];
	/************************************************************************/
	/* 索引型数据                                                                                        */
	/************************************************************************/
	//通道选择标志
	int m_nChannelIndex;
	//直方图索引地址
	int m_nHistogramIndex;
	//灰度条索引地址
	int m_nBarIndex;
	//SliderXLeft索引地址
	int m_nSliderXLeftIndex;
	//SliderXRight索引地址
	int m_nSliderXRightIndex;
	//SliderXInflection索引地址
	int m_nSliderXInflectionIndex;
	//SliderYTop索引地址
	int m_nSliderYTopIndex;
	//SliderYBottom索引地址
	int m_nSliderYBottomIndex;

};


