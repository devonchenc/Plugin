// LevelWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "Level.h"
#include "LevelWnd.h"
#include <cmath>

// CLevelWnd

IMPLEMENT_DYNAMIC(CLevelWnd, CWnd)

	CLevelWnd::CLevelWnd()
{
	m_CtrlFont.CreatePointFont(90, _T("宋体"));
	m_nHistogramIndex = 0;
	m_HistogramRect = CRect(40, 60, 320, 240);
	m_nBarIndex = 1;
	m_nChannelIndex = 3;
	m_nSliderXLeftIndex = 7;
	m_nSliderXRightIndex = 8;
	m_nSliderXInflectionIndex = 9;
	m_nSliderYTopIndex = 10;
	m_nSliderYBottomIndex = 11;
	memset(m_arrGrayValues, 0, 1024*sizeof(int));
	InitGrayMap();
	m_Items.push_back(CItemInfo(LEVEL_HISTOGRAM, LEFT_CHANGE, m_HistogramRect));//直方图0
	m_Items.push_back(CItemInfo(LEVEL_BAR, BOTTOM_CHANGE, CRect(40, 300, 320, 310)));//灰度条1
	m_Items.push_back(CItemInfo(LEVEL_COMBOBOX, LEFT_STATIC,CRect(85, 8, 145, 40), IDC_CMB_CHANNEL, IDS_RGB, 4));//通道选择2
	InitMapRect(m_Items.at(m_nHistogramIndex).m_Rect);
	m_Items.push_back(CItemInfo(LEVEL_BUTTON, LEFT_STATIC, CRect(165,7,215,29), IDC_BTN_RESET));//重置3
	m_Items.push_back(CItemInfo(LEVEL_TEXT, LEFT_STATIC, CRect(30, 12, 80, 27), IDC_STT_CHANNEL));//通道：4
	m_Items.push_back(CItemInfo(LEVEL_TEXT, LEFT_STATIC, CRect(30, 40, 120, 55), IDC_STT_INPUT));//输入：5
	m_Items.push_back(CItemInfo(LEVEL_TEXT, BOTTOM_STATIC, CRect(30, 275,120,290), IDC_STT_OUTPUT));//输出：6
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(34, 240, 48, 250), IDC_SLDTRN_XLEFT,0,0,COL_BLACK, m_nHistogramIndex, FALSE, 0, m_nSliderXRightIndex, TRUE, 3/*m_nSliderXInflectionIndex, TRUE, 3*/));//x左三角7
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(314, 240, 328, 250), IDC_SLDTRN_XRIGHT,0,0,COL_WHITE, m_nSliderXLeftIndex, TRUE, 3, m_nHistogramIndex, FALSE, 0));//x右三角8
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(174, 240, 188, 250), IDC_SLDTRN_XINFLECTION,0,0,COL_GRAY, m_nSliderXLeftIndex, TRUE, 3, m_nSliderXRightIndex, TRUE, 2));//x中三角9
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(34, 310, 48, 320), IDC_SLDTRN_YTOP,0,0,COL_BLACK, m_nBarIndex, FALSE, 0, m_nBarIndex, FALSE, 0));//y左三角10
	m_Items.push_back(CItemInfo(LEVEL_SLDWND, BOTTOM_STATIC, CRect(314, 310, 328, 320), IDC_SLDTRN_YBOTTOM,0,0,COL_WHITE, m_nBarIndex, FALSE, 0, m_nBarIndex, FALSE, 0));//y右三角11
	m_Items.push_back(CItemInfo(LEVEL_EDIT, BOTTOM_STATIC, CRect(35, 255, 60, 270), IDC_EDT_XLEFT));//x左输入12
	m_Items.push_back(CItemInfo(LEVEL_EDIT, BOTTOM_RIGHT, CRect(300, 255, 325, 270), IDC_EDT_XRIGHT));//x右输入13
	m_Items.push_back(CItemInfo(LEVEL_EDIT, BOTTOM_MEDIAN, CRect(168, 255, 193, 270), IDC_EDT_PROPORTION));//x中输入14
	m_Items.push_back(CItemInfo(LEVEL_EDIT, BOTTOM_STATIC, CRect(35, 325, 60, 340), IDC_EDT_YTOP));//y左输入15
	m_Items.push_back(CItemInfo(LEVEL_EDIT, BOTTOM_RIGHT, CRect(300, 325, 325, 340), IDC_EDT_YBOTTOM));//y右输入16
}

CLevelWnd::~CLevelWnd()
{
	for (UINT nIndex=0; nIndex<m_Items.size(); nIndex++)
	{
		if (m_Items.at(nIndex).m_pItem)
		{
			delete m_Items.at(nIndex).m_pItem;
		}
	}
}

BEGIN_MESSAGE_MAP(CLevelWnd, CWnd)
	ON_WM_PAINT()
	ON_WM_CREATE()
	ON_CBN_SELCHANGE(IDC_CMB_CHANNEL, &CLevelWnd::OnSelchangeComboChannel)
	ON_BN_CLICKED(IDC_BTN_RESET, &CLevelWnd::OnButtonReset)
	ON_EN_CHANGE(IDC_EDT_XLEFT, &CLevelWnd::OnEditXLeft)
	ON_EN_CHANGE(IDC_EDT_XRIGHT, &CLevelWnd::OnEditXRight)
	ON_EN_CHANGE(IDC_EDT_PROPORTION, &CLevelWnd::OnEditProportion)
	ON_EN_CHANGE(IDC_EDT_YTOP, &CLevelWnd::OnEditYTop)
	ON_EN_CHANGE(IDC_EDT_YBOTTOM, &CLevelWnd::OnEditYBottom)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_MBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_MOUSEWHEEL()
	ON_WM_SIZE()
END_MESSAGE_MAP()

// CLevelWnd 消息处理程序

/************************************************************************/
/* 消息响应型函数                                                                                 */
/************************************************************************/

//通道选择Combobox
void CLevelWnd::OnSelchangeComboChannel()
{
	switch(((CComboBox*)GetDlgItem(IDC_CMB_CHANNEL))->GetCurSel())
	{
	case 0://RGB
		if (m_nChannelIndex != 3)
		{
			OnButtonReset();
		}
		m_nChannelIndex = 3;
		break;
	case 1://R
		if (m_nChannelIndex == 3)
		{
			m_fArrProportion[0] = m_fArrProportion[1] = m_fArrProportion[2] = m_fArrProportion[3];
			m_strArrProportion[0] = m_strArrProportion[1] = m_strArrProportion[2] = m_strArrProportion[3];
			m_arrLevelMapRect[0] = m_arrLevelMapRect[1] = m_arrLevelMapRect[2] = m_arrLevelMapRect[3];
			for (int nIndex=0; nIndex<3; nIndex++)
			{
				m_nChannelIndex = nIndex;
				XLeftGrayToCoordinate();
				XRightGrayToCoordinate();
				ProportionToXInflection();
				YTopGrayToCoordinate();
				YBottomGrayToCoordinate();
				XLeftChangeCoordinate();
				XRightChangeCoordinate();
				XInflectionChangeCoordinate();
				YTopChangeCoordinate();
				YBottomChangeCoordinate();
			}
		}
		m_nChannelIndex = 2;
		break;
	case 2://G
		if (m_nChannelIndex == 3)
		{
			m_fArrProportion[0] = m_fArrProportion[1] = m_fArrProportion[2] = m_fArrProportion[3];
			m_strArrProportion[0] = m_strArrProportion[1] = m_strArrProportion[2] = m_strArrProportion[3];
			m_arrLevelMapRect[0] = m_arrLevelMapRect[1] = m_arrLevelMapRect[2] = m_arrLevelMapRect[3];
			for (int nIndex=0; nIndex<3; nIndex++)
			{
				m_nChannelIndex = nIndex;
				XLeftGrayToCoordinate();
				XRightGrayToCoordinate();
				ProportionToXInflection();
				YTopGrayToCoordinate();
				YBottomGrayToCoordinate();
				XLeftChangeCoordinate();
				XRightChangeCoordinate();
				XInflectionChangeCoordinate();
				YTopChangeCoordinate();
				YBottomChangeCoordinate();
			}
		}
		m_nChannelIndex = 1;
		break;
	case 3://B
		if (m_nChannelIndex == 3)
		{
			m_fArrProportion[0] = m_fArrProportion[1] = m_fArrProportion[2] = m_fArrProportion[3];
			m_strArrProportion[0] = m_strArrProportion[1] = m_strArrProportion[2] = m_strArrProportion[3];
			m_arrLevelMapRect[0] = m_arrLevelMapRect[1] = m_arrLevelMapRect[2] = m_arrLevelMapRect[3];
			for (int nIndex=0; nIndex<3; nIndex++)
			{
				m_nChannelIndex = nIndex;
				XLeftGrayToCoordinate();
				XRightGrayToCoordinate();
				ProportionToXInflection();
				YTopGrayToCoordinate();
				YBottomGrayToCoordinate();
				XLeftChangeCoordinate();
				XRightChangeCoordinate();
				XInflectionChangeCoordinate();
				YTopChangeCoordinate();
				YBottomChangeCoordinate();
			}
		}
		m_nChannelIndex = 0;
		break;
	default:
		break;
	}
	UpdateData(FALSE);
	XLeftChangeCoordinate();
	XRightChangeCoordinate();
	XInflectionChangeCoordinate();
	YTopChangeCoordinate();
	YBottomChangeCoordinate();
	Invalidate();
}

//重置Button
void CLevelWnd::OnButtonReset()
{
	if (ImageCopy(m_CopyImage, *m_pImage))
	{
		PIGetActiveView()->Invalidate();
		InitGrayMap();
		InitMapRect(m_Items.at(m_nHistogramIndex).m_Rect);
		UpdateData(FALSE);
		XLeftChangeCoordinate();
		XRightChangeCoordinate();
		XInflectionChangeCoordinate();
		YTopChangeCoordinate();
		YBottomChangeCoordinate();
	}
}

//XLeft文本编辑消息响应
void CLevelWnd::OnEditXLeft()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strLevelMapLeft[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strLevelMapLeft[m_nChannelIndex]))
		{
			int nStr = int(_ttoi(m_strLevelMapLeft[m_nChannelIndex]));
			if (nStr < int(m_fMinMatrix[m_nChannelIndex]))
			{
				m_arrLevelMapRect[m_nChannelIndex].left = LONG(m_fMinMatrix[m_nChannelIndex]);
				m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
				UpdateData(FALSE);
			}
			else if (nStr>m_arrLevelMapRect[m_nChannelIndex].right-2)
			{
				bUpData = false;
			}
			else
			{
				m_arrLevelMapRect[m_nChannelIndex].left = nStr;
			}
		}
		else
		{
			bUpData = false;
			m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
			UpdateData(FALSE);
		}
		XLeftGrayToCoordinate();
		XLeftChangeCoordinate();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//XLeft文本编辑回车响应
void CLevelWnd::EditXLeft()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strLevelMapLeft[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strLevelMapLeft[m_nChannelIndex]))
		{
			int nStr = int(_ttoi(m_strLevelMapLeft[m_nChannelIndex]));
			if (nStr < int(m_fMinMatrix[m_nChannelIndex]))
			{
				m_arrLevelMapRect[m_nChannelIndex].left = LONG(m_fMinMatrix[m_nChannelIndex]);
				m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
				UpdateData(FALSE);
			}
			else if (nStr > m_arrLevelMapRect[m_nChannelIndex].right-2)
			{
				m_arrLevelMapRect[m_nChannelIndex].left = m_arrLevelMapRect[m_nChannelIndex].right-2;
				m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
				UpdateData(FALSE);
			}
			else
			{
				m_arrLevelMapRect[m_nChannelIndex].left = nStr;
			}
		}
		else
		{
			bUpData = false;
			m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
			UpdateData(FALSE);
		}
		XLeftGrayToCoordinate();
		XLeftChangeCoordinate();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//XRight文本编辑消息响应
void CLevelWnd::OnEditXRight()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strLevelMapRight[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strLevelMapRight[m_nChannelIndex]))
		{
			int nStr = int(_ttoi(m_strLevelMapRight[m_nChannelIndex]));
			if (nStr > int(m_fMaxMatrix[m_nChannelIndex]))
			{
				m_arrLevelMapRect[m_nChannelIndex].right = LONG(m_fMaxMatrix[m_nChannelIndex]);
				m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
				UpdateData(FALSE);
			}
			else if (nStr < m_arrLevelMapRect[m_nChannelIndex].left+2)
			{
				bUpData = false;
			}
			else
			{
				m_arrLevelMapRect[m_nChannelIndex].right = nStr;
			}
		}
		else
		{
			bUpData = false;
			m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
			UpdateData(FALSE);
		}
		XRightGrayToCoordinate();
		XRightChangeCoordinate();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//XRight文本编辑回车响应
void CLevelWnd::EditXRight()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strLevelMapRight[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strLevelMapRight[m_nChannelIndex]))
		{
			int nStr = int(_ttoi(m_strLevelMapRight[m_nChannelIndex]));
			if (nStr > int(m_fMaxMatrix[m_nChannelIndex]))
			{
				m_arrLevelMapRect[m_nChannelIndex].right = LONG(m_fMaxMatrix[m_nChannelIndex]);
				m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
				UpdateData(FALSE);
			}
			else if (nStr < m_arrLevelMapRect[m_nChannelIndex].left+2)
			{
				m_arrLevelMapRect[m_nChannelIndex].right = m_arrLevelMapRect[m_nChannelIndex].left+2;
				m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
				UpdateData(FALSE);
			}
			else
			{
				m_arrLevelMapRect[m_nChannelIndex].right = nStr;
			}
		}
		else
		{
			bUpData = false;
			m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
			UpdateData(FALSE);
		}
		XRightGrayToCoordinate();
		XRightChangeCoordinate();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//Proportion文本编辑
void CLevelWnd::OnEditProportion()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strArrProportion[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strArrProportion[m_nChannelIndex]))
		{
			float fStr = (float)_tstof(m_strArrProportion[m_nChannelIndex]);
			if (fStr == 0)
			{
				bUpData = false;
			}
			else if (fStr < 0.01f)
			{
				AfxMessageBox(_T("请输入0.01至99.99之间的数"));
				m_fArrProportion[m_nChannelIndex] = 0.01f;
				m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
				UpdateData(FALSE);
			}
			else if (fStr > 99.99f)
			{
				AfxMessageBox(_T("请输入0.01至99.99之间的数"));
				m_fArrProportion[m_nChannelIndex] = 99.99f;
				m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
				UpdateData(FALSE);
			}
			else
			{
				m_fArrProportion[m_nChannelIndex] = fStr;
			}
		}
		else
		{
			bUpData = false;
			m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
			UpdateData(FALSE);
		}
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//Proportion回车响应
void CLevelWnd::EditProportion()
{
	UpdateData(TRUE);
	if (CheckNum(m_strArrProportion[m_nChannelIndex]))
	{
		float fStr =  (float)_tstof(m_strArrProportion[m_nChannelIndex]);
		if (fStr < 0.01f)
		{
			AfxMessageBox(_T("请输入0.01至99.99之间的数"));
			m_fArrProportion[m_nChannelIndex] = 0.01f;
		}
		else if (fStr > 99.99f)
		{
			AfxMessageBox(_T("请输入0.01至99.99之间的数"));
			m_fArrProportion[m_nChannelIndex] = 99.99f;
		}
		else
		{
			m_fArrProportion[m_nChannelIndex] = fStr;
		}
	}
	m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
	UpdateData(FALSE);
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}

//YTop文本编辑
void CLevelWnd::OnEditYTop()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strLevelMapTop[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strLevelMapTop[m_nChannelIndex]))
		{
			int nStr = (int)_ttoi(m_strLevelMapTop[m_nChannelIndex]);
			if (nStr < 0)
			{
				m_arrLevelMapRect[m_nChannelIndex].top = 0;
				m_strLevelMapTop[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].top);
				UpdateData(FALSE);
			}
			else if (nStr > 255)
			{
				m_arrLevelMapRect[m_nChannelIndex].top = 255;
				m_strLevelMapTop[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].top);
				UpdateData(FALSE);
			}
			else
			{
				m_arrLevelMapRect[m_nChannelIndex].top = nStr;
			}
		}
		else
		{
			bUpData = false;
			m_strLevelMapTop[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].top);
			UpdateData(FALSE);
		}
		YTopGrayToCoordinate();
		YTopChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//YBottom文本编辑
void CLevelWnd::OnEditYBottom()
{
	bool bUpData = true;
	UpdateData(TRUE);
	if (!m_strLevelMapBottom[m_nChannelIndex].IsEmpty())
	{
		if (CheckNum(m_strLevelMapBottom[m_nChannelIndex]))
		{
			int nStr = (int)_ttoi(m_strLevelMapBottom[m_nChannelIndex]);
			if (nStr < 0)
			{
				m_arrLevelMapRect[m_nChannelIndex].bottom = 0;
				m_strLevelMapBottom[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].bottom);
				UpdateData(FALSE);
			}
			else if (nStr > 255)
			{
				m_arrLevelMapRect[m_nChannelIndex].bottom = 255;
				m_strLevelMapBottom[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].bottom);
				UpdateData(FALSE);
			}
			else
			{
				m_arrLevelMapRect[m_nChannelIndex].bottom = nStr;
			}
		}
		else
		{
			bUpData = false;
			m_strLevelMapBottom[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].bottom);
			UpdateData(FALSE);
		}
		YBottomGrayToCoordinate();
		YBottomChangeCoordinate();
		GrayMapping();
		if (bUpData)
		{
			UpdateImage();
		}
		Invalidate();
	}
}

//初始化
int CLevelWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	GetClientRect(&m_rect);
	NewItems();
	ReSize(0,m_rect);
	InitGrayValues();
	UpdateData(FALSE);
	return 0;
}

//窗口重绘
void CLevelWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect rect;
	GetClientRect(&rect); // 得到按钮窗口的有效矩形区域

	CDC memDC;//创建内存DC
	memDC.CreateCompatibleDC(&dc);
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(&dc,rect.Width(),rect.Height());
	memDC.SelectObject(&bitmap);

	memDC.FillSolidRect(&rect, GetSysColor(COLOR_3DFACE));
	DrawHistogram(memDC);
	DrawBar(memDC);
	dc.BitBlt(rect.left, rect.top, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	bitmap.DeleteObject();
	memDC.DeleteDC();
}

//背景擦除
BOOL CLevelWnd::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//	return CWnd::OnEraseBkgnd(pDC);
	return FALSE;
}

//数据交换
void CLevelWnd::DoDataExchange(CDataExchange* pDX)
{
	// TODO: 在此添加专用代码和/或调用基类
	CWnd::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDT_XLEFT, m_strLevelMapLeft[m_nChannelIndex]);
	DDX_Text(pDX, IDC_EDT_XRIGHT, m_strLevelMapRight[m_nChannelIndex]);
	DDX_Text(pDX, IDC_EDT_PROPORTION, m_strArrProportion[m_nChannelIndex]);
	DDX_Text(pDX, IDC_EDT_YTOP, m_strLevelMapTop[m_nChannelIndex]);
	DDX_Text(pDX, IDC_EDT_YBOTTOM, m_strLevelMapBottom[m_nChannelIndex]);
}

//响应Dlg的WM_SIZE
void CLevelWnd::ReSize(const UINT& nType, const CRect& newRect)
{
	if (nType != SIZE_MINIMIZED)
	{
		MoveWindow(newRect);
		for (UINT nIndex=0; nIndex<m_Items.size(); nIndex++)
		{
			if (SetItemPosition(newRect, nIndex))
			{
				switch (nIndex-m_nSliderXLeftIndex)
				{
				case 0:
					XLeftGrayToCoordinate();
					XLeftChangeCoordinate();
					break;
				case 1:
					XRightGrayToCoordinate();
					XRightChangeCoordinate();
					break;
				case 2:
					ProportionToXInflection();
					XInflectionChangeCoordinate();
					break;
				case 3:
					YTopGrayToCoordinate();
					YTopChangeCoordinate();
					break;
				case 4:
					YBottomGrayToCoordinate();
					YBottomChangeCoordinate();
					break;
				default:
					if (m_Items.at(nIndex).m_pItem != NULL)
					{
						m_Items.at(nIndex).m_pItem->MoveWindow(m_Items.at(nIndex).m_Rect);
					}
					break;
				}
			}
		}
		int nChannelIndex = m_nChannelIndex;
		for (m_nChannelIndex=0; m_nChannelIndex<4; m_nChannelIndex++)
		{
			XLeftGrayToCoordinate();
			XLeftChangeCoordinate();
			XRightGrayToCoordinate();
			XRightChangeCoordinate();
			ProportionToXInflection();
			XInflectionChangeCoordinate();
			YTopGrayToCoordinate();
			YTopChangeCoordinate();
			YBottomGrayToCoordinate();
			YBottomChangeCoordinate();
		}
		m_nChannelIndex = nChannelIndex;
		m_rect = newRect;
		m_HistogramRect = m_Items.at(m_nHistogramIndex).m_Rect;
	}
}

//消息预处理
BOOL CLevelWnd::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	// 	if (pMsg-> message==WM_KEYDOWN && pMsg-> wParam==VK_ESCAPE) 
	// 	{ 
	// 		return TRUE; 
	// 	} 
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
	{ 
		if (GetFocus()->GetDlgCtrlID() == IDC_EDT_PROPORTION)
		{
			EditProportion();
		}
		if (GetFocus()->GetDlgCtrlID() == IDC_EDT_XLEFT)
		{
			EditXLeft();
		}
		if (GetFocus()->GetDlgCtrlID() == IDC_EDT_XRIGHT)
		{
			EditXRight();
		}
		return TRUE; 
	} 
	else if (pMsg->message == WM_LBUTTONDOWN || pMsg->message == WM_RBUTTONDOWN || pMsg->message == WM_MBUTTONDOWN)
	{
		CPoint point;
		GetCursorPos(&point);
		CRect rect1;
		CRect rect2;
		GetDlgItem( IDC_EDT_XLEFT)->GetWindowRect(&rect1);
		GetDlgItem(IDC_SLDTRN_XLEFT)->GetWindowRect(&rect2);
		if (rect1.PtInRect(point) || rect2.PtInRect(point))
		{
			GetDlgItem( IDC_EDT_XLEFT)->SetFocus();
			m_nFocusID = IDC_EDT_XLEFT;
		}
		GetDlgItem( IDC_EDT_XRIGHT)->GetWindowRect(&rect1);
		GetDlgItem(IDC_SLDTRN_XRIGHT)->GetWindowRect(&rect2);
		if (rect1.PtInRect(point) || rect2.PtInRect(point))
		{
			GetDlgItem(IDC_EDT_XRIGHT)->SetFocus();
			m_nFocusID = IDC_EDT_XRIGHT;
		}
		GetDlgItem( IDC_EDT_YTOP)->GetWindowRect(&rect1);
		GetDlgItem(IDC_SLDTRN_YTOP)->GetWindowRect(&rect2);
		if (rect1.PtInRect(point) || rect2.PtInRect(point))
		{
			GetDlgItem(IDC_EDT_YTOP)->SetFocus();
			m_nFocusID = IDC_EDT_YTOP;
		}
		GetDlgItem( IDC_EDT_YBOTTOM)->GetWindowRect(&rect1);
		GetDlgItem(IDC_SLDTRN_YBOTTOM)->GetWindowRect(&rect2);
		if (rect1.PtInRect(point) || rect2.PtInRect(point))
		{
			GetDlgItem( IDC_EDT_YBOTTOM)->SetFocus();
			m_nFocusID = IDC_EDT_YBOTTOM;
		}
		GetDlgItem( IDC_EDT_PROPORTION)->GetWindowRect(&rect1);
		GetDlgItem(IDC_SLDTRN_XINFLECTION)->GetWindowRect(&rect2);
		if (rect1.PtInRect(point) || rect2.PtInRect(point))
		{
			GetDlgItem( IDC_EDT_PROPORTION)->SetFocus();
			m_nFocusID = IDC_EDT_PROPORTION;
		}
		else
		{
			SetFocus();
		}
		return CWnd::PreTranslateMessage(pMsg); 
	}
	else if (pMsg->message == WM_MOUSEWHEEL)
	{
		SetFocus();
		return CWnd::PreTranslateMessage(pMsg); 
	}
	else 
	{ 			
		return CWnd::PreTranslateMessage(pMsg); 
	} 
}

//响应父窗口WM_SIZE
void CLevelWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: 在此处添加消息处理程序代码
	if (GetSafeHwnd() != NULL)
	{
		ReSize(nType, CRect(0,0,cx,cy));
 	}
}

//响应鼠标左键消息
void CLevelWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nFocusID = GetFocus()->GetDlgCtrlID();
	SetFocus();
	MouseDown(point);
	CWnd::OnLButtonDown(nFlags, point);
}

//响应鼠标中键消息
void CLevelWnd::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nFocusID = GetFocus()->GetDlgCtrlID();
	SetFocus();
	MouseDown(point);
	CWnd::OnMButtonDown(nFlags, point);
}

//响应鼠标右键消息
void CLevelWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_nFocusID = GetFocus()->GetDlgCtrlID();
	SetFocus();
	MouseDown(point);
	CWnd::OnRButtonDown(nFlags, point);
}

//鼠标按键内部实现
void CLevelWnd::MouseDown(CPoint point)
{
	CRect rect;
	GetDlgItem( IDC_EDT_XLEFT)->GetWindowRect(&rect);
	if (!rect.PtInRect(point))
	{
		CString str;
		GetDlgItem(IDC_EDT_XLEFT)->GetWindowText(str);
		if (str.IsEmpty())
		{
			m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
			SetDlgItemText(IDC_EDT_XLEFT, m_strLevelMapLeft[m_nChannelIndex]);
		}
		else
		{
			EditXLeft();
		}
	}
	GetDlgItem( IDC_EDT_XRIGHT)->GetWindowRect(&rect);
	if (!rect.PtInRect(point))
	{
		CString str;
		GetDlgItem(IDC_EDT_XRIGHT)->GetWindowText(str);
		if (str.IsEmpty())
		{
			m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
			SetDlgItemText(IDC_EDT_XRIGHT, m_strLevelMapRight[m_nChannelIndex]);
		}
		else
		{
			EditXRight();
		}
	}
	GetDlgItem( IDC_EDT_YTOP)->GetWindowRect(&rect);
	if (!rect.PtInRect(point))
	{
		CString str;
		GetDlgItem(IDC_EDT_YTOP)->GetWindowText(str);
		if (str.IsEmpty())
		{
			m_strLevelMapTop[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].top);
			SetDlgItemText(IDC_EDT_YTOP, m_strLevelMapTop[m_nChannelIndex]);
		}
	}
	GetDlgItem( IDC_EDT_YBOTTOM)->GetWindowRect(&rect);
	if (!rect.PtInRect(point))
	{
		CString str;
		GetDlgItem(IDC_EDT_YBOTTOM)->GetWindowText(str);
		if (str.IsEmpty())
		{
			m_strLevelMapBottom[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].bottom);
			SetDlgItemText(IDC_EDT_YBOTTOM, m_strLevelMapBottom[m_nChannelIndex]);
		}
	}
	GetDlgItem( IDC_EDT_PROPORTION)->GetWindowRect(&rect);
	if (!rect.PtInRect(point))
	{
		CString str;
		GetDlgItem(IDC_EDT_PROPORTION)->GetWindowText(str);
		if (str.IsEmpty())
		{
			m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
			SetDlgItemText(IDC_EDT_PROPORTION, m_strArrProportion[m_nChannelIndex]);
		}
		else
		{
			EditProportion();
		}
	}
}

//响应鼠标滚轮消息
BOOL CLevelWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_nFocusID == IDC_EDT_PROPORTION)
	{
		WheelProportion(nFlags, 0.0005f*zDelta);
	}
	else if (m_nFocusID == IDC_EDT_XLEFT)
	{
		WheelXLeft(nFlags, 0.01f*zDelta);
	}
	else if (m_nFocusID == IDC_EDT_XRIGHT)
	{
		WheelXRight(nFlags, 0.01f*zDelta);
	}
	else if (m_nFocusID == IDC_EDT_YTOP)
	{
		WheelYTop(nFlags, 0.01f*zDelta);
	}
	else if (m_nFocusID == IDC_EDT_YBOTTOM)
	{
		WheelYBottom(nFlags, 0.01f*zDelta);
	}
	return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}

//鼠标滚轮XLeft响应
void CLevelWnd::WheelXLeft(UINT nFlags, float zDelta)
{
	int nStr = m_arrLevelMapRect[m_nChannelIndex].left;
	if (nFlags)
	{
		nStr -= int(zDelta);
	}
	else
	{
		nStr += int(zDelta);
	}
	nStr = nStr>m_fMinMatrix[m_nChannelIndex]?nStr:LONG(m_fMinMatrix[m_nChannelIndex]);
	nStr = nStr<m_arrLevelMapRect[m_nChannelIndex].right-2?nStr:m_arrLevelMapRect[m_nChannelIndex].right-2;
	m_arrLevelMapRect[m_nChannelIndex].left = nStr;
	m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
	UpdateData(FALSE);
	XLeftGrayToCoordinate();
	XLeftChangeCoordinate();
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}

//鼠标滚轮XRight响应
void CLevelWnd::WheelXRight(UINT nFlags, float zDelta)
{
	int nStr = m_arrLevelMapRect[m_nChannelIndex].right;
	if (nFlags)
	{
		nStr -= int(zDelta);
	}
	else
	{
		nStr += int(zDelta);
	}
	nStr = nStr<m_fMaxMatrix[m_nChannelIndex]?nStr:LONG(m_fMaxMatrix[m_nChannelIndex]);
	nStr = nStr>m_arrLevelMapRect[m_nChannelIndex].left+2?nStr:m_arrLevelMapRect[m_nChannelIndex].left+2;
	m_arrLevelMapRect[m_nChannelIndex].right = nStr;
	m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
	UpdateData(FALSE);
	XRightGrayToCoordinate();
	XRightChangeCoordinate();
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}

//鼠标滚轮XProportion响应
void CLevelWnd::WheelProportion(UINT nFlags, float zDelta)
{
	float fStr = m_fArrProportion[m_nChannelIndex];
	if (nFlags)
	{
		fStr -= zDelta;
	}
	else
	{
		fStr += zDelta;
	}
	fStr = fStr>0.01f?fStr:0.01f;
	fStr = fStr<99.99f?fStr:99.99f;
	m_fArrProportion[m_nChannelIndex] = fStr;
	m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
	UpdateData(FALSE);
	ProportionToXInflection();
	XInflectionChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}

//鼠标滚轮YTop响应
void CLevelWnd::WheelYTop(UINT nFlags, float zDelta)
{
	int nStr = m_arrLevelMapRect[m_nChannelIndex].top;
	if (nFlags)
	{
		nStr -= int(zDelta);
	}
	else
	{
		nStr += int(zDelta);
	}
	nStr = nStr>0?nStr:0;
	nStr = nStr<255?nStr:255;
	m_arrLevelMapRect[m_nChannelIndex].top = nStr;
	m_strLevelMapTop[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].top);
	UpdateData(FALSE);
	YTopGrayToCoordinate();
	YTopChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}

//鼠标滚轮YBottom响应
void CLevelWnd::WheelYBottom(UINT nFlags, float zDelta)
{
	int nStr = m_arrLevelMapRect[m_nChannelIndex].bottom;
	if (nFlags)
	{
		nStr -= int(zDelta);
	}
	else
	{
		nStr += int(zDelta);
	}
	nStr = nStr>0?nStr:0;
	nStr = nStr<255?nStr:255;
	m_arrLevelMapRect[m_nChannelIndex].bottom = nStr;
	m_strLevelMapBottom[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].bottom);
	UpdateData(FALSE);
	YBottomGrayToCoordinate();
	YBottomChangeCoordinate();
	GrayMapping();
	UpdateImage();
	Invalidate();
}

/************************************************************************/
/*数据传递型函数                                                                                  */
/************************************************************************/

//获取图片
CImage* CLevelWnd::GetImage()
{
	CDocument* pDoc = PIGetActiveDocument();
	if (pDoc == NULL)
		return NULL;
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CPIDocument)))
		return NULL;

	return((CPIDocument*)pDoc)->GetImage();
}

//初始化灰度直方图
void CLevelWnd::InitGrayValues()
{
	m_pImage = GetImage();
	ImageCopy(*m_pImage, m_CopyImage);
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();
	BYTE* pImageData = (BYTE*)m_pImage->GetBits();
	int nPitch = m_pImage->GetPitch();
	int nBitCount = m_pImage->GetBPP() / 8;
	for (int t=0; t<4; t++)
	{
		m_arrLevelMapRect[t].left = LONG(m_fMinMatrix[t]);
		m_arrLevelMapRect[t].right = LONG(m_fMaxMatrix[t]);
		if (m_fMaxMatrix[t]!=m_fMinMatrix[t])
		{
			m_fTranslateProportion[t] = 256.0f/(m_fMaxMatrix[t]-m_fMinMatrix[t]+1);
		}
		else
		{
			m_fTranslateProportion[t] = NULL;
		}
	}
	for (int j=0; j<nHeight; j++)
	{
		for (int i=0; i<nWidth; i++)
		{
			int nPixelIndex = j * nPitch + i * nBitCount;
			BYTE* pPixel = pImageData +nPixelIndex;
			for (int nIndex=0; nIndex<4; nIndex++)
			{
				if (nIndex != 3)
				{
					if (m_fTranslateProportion[nIndex] != NULL)
					{
						++m_arrGrayValues[nIndex][int(((*(pPixel+nIndex))-m_fMinMatrix[nIndex])*m_fTranslateProportion[nIndex])];
					}
					else
					{
						++m_arrGrayValues[nIndex][int(m_fMinMatrix[nIndex])];
					}
				}
				else
				{
					if (m_fTranslateProportion[nIndex]!=NULL)
					{
						++m_arrGrayValues[nIndex][int((((*(pPixel)+0.5)*0.3+(*(pPixel+1)+0.5)*0.59+(*(pPixel+2)+0.5)*0.11)-m_fMinMatrix[nIndex])*m_fTranslateProportion[nIndex])];
					}
					else
					{
						++m_arrGrayValues[nIndex][int(m_fMinMatrix[nIndex])];
					}
				}
			}
		}
	}
	float ftScale = 1.0f/(nWidth*nHeight);
	float ftLog = 1/log(1.03f);
	for (int k=0; k<256; k++)
	{
		m_arrGrayValues[0][k] = log(1+m_arrGrayValues[0][k]*ftScale)*ftLog;
		m_arrGrayValues[1][k] = log(1+m_arrGrayValues[1][k]*ftScale)*ftLog;
		m_arrGrayValues[2][k] = log(1+m_arrGrayValues[2][k]*ftScale)*ftLog;
		m_arrGrayValues[3][k] = log(1+m_arrGrayValues[3][k]*ftScale)*ftLog;
	}
}

//初始化灰度映射表
void CLevelWnd::InitGrayMap()
{
	for (int nIndex=0; nIndex<256; nIndex++)
	{
		m_arrLevelMap[0][nIndex] = nIndex;
		m_arrLevelMap[1][nIndex] = nIndex;
		m_arrLevelMap[2][nIndex] = nIndex;
		m_arrLevelMap[3][nIndex] = nIndex;
	}
}

//初始化几何&灰度映射范围
void CLevelWnd::InitMapRect(const CRect& rect)
{
	for (int nIndex=0; nIndex<4; nIndex++)
	{
		m_arrCoordinateRact[nIndex] = CRect(rect.left, rect.left, rect.right, rect.right) ;
		m_arrXInflection[nIndex] = int(m_arrCoordinateRact[nIndex].left+0.5*m_arrCoordinateRact[nIndex].Width());
		m_fArrProportion[nIndex] = 1.00f;
		m_strArrProportion[nIndex].Format(_T("%.2f"), m_fArrProportion[nIndex]);
		m_fMinMatrix[nIndex] = 0;
		m_fMaxMatrix[nIndex]  = 255;
		m_arrLevelMapRect[nIndex] = CRect(0,0,255,255);
		m_strLevelMapLeft[nIndex].Format(_T("%d"), m_arrLevelMapRect[nIndex].left);
		m_strLevelMapRight[nIndex].Format(_T("%d"), m_arrLevelMapRect[nIndex].right);
		m_strLevelMapTop[nIndex].Format(_T("%d"), m_arrLevelMapRect[nIndex].top);
		m_strLevelMapBottom[nIndex].Format(_T("%d"), m_arrLevelMapRect[nIndex].bottom);
		m_fTranslateProportion[nIndex]  = 1;
	}
}

//绘制直方图
void CLevelWnd::DrawHistogram(CDC& memDC)
{
	CRect drawRect = &m_Items.at(m_nHistogramIndex).m_Rect;
	CPen pen(PS_SOLID, 1, RGB(200, 200, 200));
	CBrush brush(GetSysColor(COLOR_3DFACE));
	memDC.SelectObject(pen);
	memDC.SelectObject(brush);
	memDC.Rectangle(drawRect);
	float fStepLength = drawRect.Width()/256.0f;
	float fLeft = 1.0f*drawRect.left;
	int nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][0]*drawRect.Height());
	nTop = nTop > drawRect.top ? nTop : drawRect.top;
	memDC.FillSolidRect(CRect(int(fLeft), nTop, int(fLeft+fStepLength), drawRect.bottom), DrawColor());
	for (int i=1; i<255; i++, fLeft += fStepLength)
	{
		nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][i]*drawRect.Height()+0.5);
		nTop = nTop > drawRect.top ? nTop : drawRect.top;
		memDC.FillSolidRect(CRect(int(fLeft), int(nTop), int(fLeft+fStepLength), drawRect.bottom), DrawColor());
	}
	nTop = int(drawRect.bottom - m_arrGrayValues[m_nChannelIndex][255]*drawRect.Height()+0.5);
	nTop = nTop > drawRect.top ? nTop : drawRect.top;
	memDC.FillSolidRect(CRect(int(fLeft), nTop, int(fLeft+fStepLength), drawRect.bottom), DrawColor());
}

//绘制灰度条
void CLevelWnd::DrawBar(CDC& MemDC)
{
	CRect barRect = m_Items.at(m_nBarIndex).m_Rect;
	float fStepLength =barRect.Width()/256.0f;
	float left = 1.0f*barRect.left;
	for (int color=0; color<256; color++, left+=fStepLength)
	{
		CBrush brush(RGB(color, color, color));
		MemDC.FillRect(CRect(int(left), barRect.top, int(left+fStepLength), barRect.bottom), &brush);
	}
}

//设定直方图颜色
COLORREF CLevelWnd::DrawColor()
{
	switch (m_nChannelIndex)
	{
	case 0:
		return RGB(0, 0, 255);
	case 1:
		return RGB(0, 255, 0);
	case 2:
		return RGB(255, 0, 0);
	case 3:
		return RGB(0, 0, 0);
	default:
		return RGB(255, 255, 255);
	}
}

//创建控件
void CLevelWnd::NewItems()
{
	CRect rect;
	GetClientRect(&rect);
	CString str;
	for (UINT nIndex=0; nIndex<m_Items.size(); nIndex++)
	{
		const ItemType& eType = m_Items.at(nIndex).m_eType;
		const ItemPosition& ePosition = m_Items.at(nIndex).m_ePosition;
		CRect& itemRect = m_Items.at(nIndex).m_Rect;
		const UINT& nID = m_Items.at(nIndex).m_nID;
		const UINT& nSourceID = m_Items.at(nIndex).m_nSourceID;
		const int& nSourveNum = m_Items.at(nIndex).m_nSourceNum;
		const ItemExStyle& eExStyle = m_Items.at(nIndex).m_eExStyle;
		CWnd*& pItem = m_Items.at(nIndex).m_pItem;
		switch (eType)
		{
		case LEVEL_BUTTON:
			pItem = new CButton();
			str.LoadString(nID);
			((CButton*)pItem)->Create(str, WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_COMBOBOX:
			pItem = new CComboBox();
			((CComboBox*)pItem)->Create(WS_CHILD | WS_VISIBLE | CBS_DROPDOWNLIST, itemRect, this, nID);
			for (int nCount=0; nCount<nSourveNum; nCount++)
			{
				str.LoadString((nSourceID+nCount));
				((CComboBox*)pItem)->InsertString(nCount, str);
			}
			((CComboBox*)pItem)->SetCurSel(0);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_TEXT:
			pItem = new CStatic();
			str.LoadString(nID);
			((CStatic*)pItem)->Create(str, WS_CHILD | WS_VISIBLE|SS_LEFT, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_HISTOGRAM:
			break;
		case LEVEL_BAR:
			break;
		case LEVEL_EDIT:
			pItem = new CEdit();
			((CEdit*)pItem)->Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | WS_TABSTOP|ES_AUTOHSCROLL |ES_LEFT, itemRect, this, nID);
			pItem->SetFont(&m_CtrlFont);
			break;
		case LEVEL_SLDWND:
			{
				int& nMinID = m_Items.at(nIndex).m_nMinIndex;
				int& nMaxID = m_Items.at(nIndex).m_nMaxIndex;
				int& nMinDis = m_Items.at(nIndex).m_nMinDis;
				int& nMaxDis = m_Items.at(nIndex).m_nMaxDis;
				BOOL& bMinFlag = m_Items.at(nIndex).m_bMinFlag;
				BOOL& bMaxFlag = m_Items.at(nIndex).m_bMaxFlag;
				CRect* pMin = &m_Items.at(nMinID).m_Rect;
				CRect* pMax = &m_Items.at(nMaxID).m_Rect;
				pItem = new CLevelSlider();
				((CLevelSlider*)pItem)->NewLevelSlider(itemRect, nID,eExStyle, this, (CLevelSlider*)pItem, pMin, bMinFlag, nMinDis, pMax, bMaxFlag,  nMaxDis);
				pItem->SetFont(&m_CtrlFont);
				break;
			}
		default:
			break;
		}
	}
	UpdateData(FALSE);
}

//灰度映射
void CLevelWnd::GrayMapping()
{
	const CRect& mappingSpace = m_arrLevelMapRect[m_nChannelIndex];
	const float& fProportion = m_fArrProportion[m_nChannelIndex];
	int nInflectionGrayY = int(mappingSpace.top+0.5*mappingSpace.Height());
	int nInflectionGrayX, nLeftGrayX, nRightGrayX;
	if (m_fTranslateProportion[m_nChannelIndex]!=NULL)
	{
		nInflectionGrayX = int((mappingSpace.Width()*fProportion/(1.0f+fProportion)+mappingSpace.left-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex]);
		nLeftGrayX = int((mappingSpace.left-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex]);
		nRightGrayX = int((mappingSpace.right-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex]);
	}
	else
	{
		if (m_fMinMatrix[m_nChannelIndex] < 0)
		{
			nInflectionGrayX = nLeftGrayX = nRightGrayX = 0;
		}
		else if (m_fMinMatrix[m_nChannelIndex] > 255)
		{
			nInflectionGrayX = nLeftGrayX = nRightGrayX = 255;
		}
		else
		{
			nInflectionGrayX = nLeftGrayX = nRightGrayX = int(m_fMinMatrix[m_nChannelIndex]);
		}
	}
	float fStepLengthLeft = 0.5f*mappingSpace.Height()/(nInflectionGrayX-nLeftGrayX);
	float fStepLengthRight = 0.5f*mappingSpace.Height()/(nRightGrayX-nInflectionGrayX);
	for (int nCount=0; nCount<256; nCount++)
	{
		if (nCount < nLeftGrayX)
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = mappingSpace.top;
		}
		else if (nCount >= nLeftGrayX && nCount < nInflectionGrayX)
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = int(mappingSpace.top+(nCount-nLeftGrayX)*fStepLengthLeft);
		}
		else if (nCount >= nInflectionGrayX && nCount < nRightGrayX)
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = int(nInflectionGrayY+(nCount-nInflectionGrayX)*fStepLengthRight);
		}
		else
		{
			m_arrLevelMap[m_nChannelIndex][nCount] = mappingSpace.bottom;
		}
	}
}

//设置控件位置
BOOL CLevelWnd::SetItemPosition(const CRect& newRect, const UINT& nIndex)
{
	const ItemPosition& ePosition = m_Items.at(nIndex).m_ePosition;
	CRect& itemRect = m_Items.at(nIndex).m_Rect;
	switch (ePosition)
	{
	case LEFT_STATIC:
		return TRUE;
	case LEFT_CHANGE:
		{
			int nRightMargin = 160;
			int nBottomMargin = 120;
			int nWidthChange = newRect.Width()-itemRect.left-nRightMargin;
			nWidthChange = nWidthChange<=0?1:nWidthChange;
			int nHeightChange = newRect.Height()-itemRect.top-nBottomMargin;
			nHeightChange = nHeightChange<=0?1:nHeightChange;
			itemRect.right = LONG(nWidthChange)+itemRect.left;
			itemRect.bottom = LONG(nHeightChange)+itemRect.top;
			return FALSE;
		}
	case RIGHT_STATIC:
		{
			int nItemWidth = itemRect.Width();
			int nRightMargin = m_rect.right-itemRect.right;
			itemRect.left = newRect.right-nRightMargin - nItemWidth;
			itemRect.right = itemRect.left+nItemWidth;
			return TRUE;
		}
	case BOTTOM_CHANGE:
		{
			int nItemHeight = itemRect.Height();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			itemRect.right = itemRect.left+m_Items.at(m_nHistogramIndex).m_Rect.Width();
			return FALSE;
		}
	case BOTTOM_STATIC:
		{
			int nItemHeight = itemRect.Height();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			return TRUE;
		}
	case BOTTOM_MEDIAN:
		{
			int nItemHeight = itemRect.Height();
			int nItemWidth = itemRect.Width();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			itemRect.left = LONG(0.5*(m_Items.at(m_nHistogramIndex).m_Rect.Width()-itemRect.Width())+m_Items.at(m_nHistogramIndex).m_Rect.left);
			itemRect.right = itemRect.left +nItemWidth;
			return TRUE;
		}
	case BOTTOM_RIGHT:
		{
			int nItemHeight = itemRect.Height();
			int nItemWidth = itemRect.Width();
			itemRect.top = itemRect.top-m_HistogramRect.bottom+m_Items.at(m_nHistogramIndex).m_Rect.bottom;
			itemRect.bottom = itemRect.top+nItemHeight;
			itemRect.left = itemRect.left+m_Items.at(m_nHistogramIndex).m_Rect.Width()-m_HistogramRect.Width();
			itemRect.right = itemRect.left +nItemWidth;
			return TRUE;
		}
	default:
		return FALSE;
	}
}

//更新图片
BOOL CLevelWnd::UpdateImage()
{
	if (m_pImage==NULL)
	{
		return FALSE;
	}
	int nWidth = m_pImage->GetWidth();
	int nHeight = m_pImage->GetHeight();
	BYTE* pImageData = (BYTE*)m_pImage->GetBits();
	BYTE* pCopyImageData = (BYTE*)m_CopyImage.GetBits();
	int nPitch = m_pImage->GetPitch();
	int nBitCount = m_pImage->GetBPP() / 8;
	for (int j=0; j<nHeight; j++)
	{
		for (int i=0; i<nWidth; i++)
		{
			int nPixelIndex = j * nPitch + i * nBitCount;
			BYTE* pPixel = pImageData +nPixelIndex;
			BYTE* pCopyPixel = pCopyImageData +nPixelIndex;
			if (m_nChannelIndex == 3)
			{
				*(pPixel) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel))-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex])];
				*(pPixel+1) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel+1))-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex])];
				*(pPixel+2) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel+2))-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex])];			
			}
			else
			{
				*(pPixel+m_nChannelIndex) = m_arrLevelMap[m_nChannelIndex][int(((*(pCopyPixel+m_nChannelIndex))-m_fMinMatrix[m_nChannelIndex])*m_fTranslateProportion[m_nChannelIndex])];
			}
		}
	}
	PIGetActiveView()->Invalidate();
	return TRUE;
}

//拷贝图片
BOOL CLevelWnd::ImageCopy(const CImage& m_Image, CImage& m_CopyImage)
{
	if (m_Image.IsNull())
		return FALSE;
	//源图像参数
	int nWidth = m_Image.GetWidth();
	int nHeight = m_Image.GetHeight();
	BYTE* pImageData = (BYTE*)m_Image.GetBits();
	int nPitch = m_Image.GetPitch();
	int nBitCount = m_Image.GetBPP();
	//销毁原有图像
	if (!m_CopyImage.IsNull())
	{
		m_CopyImage.Destroy();
	}
	//创建新图像
	if (nBitCount == 32)   //支持alpha通道
	{
		m_CopyImage.Create(nWidth, nHeight, nBitCount, 1);
	}
	else
	{
		m_CopyImage.Create(nWidth, nHeight, nBitCount, 0);
	}
	//加载调色板
	if (nBitCount <= 8 && m_Image.IsIndexed())//需要调色板
	{
		RGBQUAD pal[256];
		int nColors=m_Image.GetMaxColorTableEntries();
		if (nColors>0)
		{     
			m_Image.GetColorTable(0,nColors,pal);
			m_CopyImage.SetColorTable(0,nColors,pal);//复制调色板程序
		}   
	} 
	//目标图像参数
	BYTE *pCopyImageData = (BYTE*)m_CopyImage.GetBits();
	int nCopyPitch = m_CopyImage.GetPitch();
	//复制图像数据
	for (int i=0; i<nHeight; i++)
	{
		memcpy( pCopyImageData+i*nCopyPitch, pImageData+i*nPitch, abs(nPitch) );
	} 
	return TRUE;
}

//判断输入
BOOL CLevelWnd::CheckNum(CString str)
{
	bool bNumFlag = true;
	bool bFloatFlag = false;
	for (int i=0; i<str.GetLength(); i++)
	{
		if (int(str.GetAt(i)) >= 48 && int(str.GetAt(i)) <= 57)
		{
				continue;
		}
		else if (str.GetAt(i) == '.')
		{
			if (bFloatFlag)
			{
				bNumFlag = false;
				break; 
			}
			else
			{
				bFloatFlag = true;
				continue;
			}
		}
		else
		{
			bNumFlag = false;
			break;
		}
	}
	return bNumFlag;
}

//坐标转灰度

//Wnd调用坐标转灰度
void CLevelWnd::CoordinateToGray(UINT nID, LONG center)
{
	switch (nID)
	{
	case IDC_SLDTRN_XLEFT:
		m_arrCoordinateRact[m_nChannelIndex].left = center;
		XLeftCoordinateToGray();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		break;
	case IDC_SLDTRN_XRIGHT:
		m_arrCoordinateRact[m_nChannelIndex].right = center;
		XRightCoordinateToGray();
		ProportionToXInflection();
		XInflectionChangeCoordinate();
		break;
	case IDC_SLDTRN_YTOP:
		m_arrCoordinateRact[m_nChannelIndex].top = center;
		YTopCoordinateToGray();
		break;
	case IDC_SLDTRN_YBOTTOM:
		m_arrCoordinateRact[m_nChannelIndex].bottom = center;
		YBottomCoordinateToGray();
		break;
	case IDC_SLDTRN_XINFLECTION:
		m_arrXInflection[m_nChannelIndex] = center;
		XInflectionToProportion();
		break;
	default:
		break;
	}
	UpdateData(FALSE);
}

//xLeft坐标转xLeft灰度值
void CLevelWnd::XLeftCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXLeftCoordinate = m_arrCoordinateRact[m_nChannelIndex].left;
	float fScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].left = int((nXLeftCoordinate-wholeRect.left)*fScale);
	m_strLevelMapLeft[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].left);
}

//xRight坐标转xRight灰度值
void CLevelWnd::XRightCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXRightCoordinate = m_arrCoordinateRact[m_nChannelIndex].right;
	float fScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].right = int((nXRightCoordinate-wholeRect.left)*fScale);
	m_strLevelMapRight[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].right);
}

//拐点x坐标转拐点比例
void CLevelWnd::XInflectionToProportion()
{
	const CRect& coordinateRact = m_arrCoordinateRact[m_nChannelIndex];
	int& nInflection_x = m_arrXInflection[m_nChannelIndex];
	m_fArrProportion[m_nChannelIndex] = 1.0f* (nInflection_x-coordinateRact.left)/(coordinateRact.right-nInflection_x);
	m_strArrProportion[m_nChannelIndex].Format(_T("%.2f"), m_fArrProportion[m_nChannelIndex]);
}

//yTop坐标转yTop灰度值
void CLevelWnd:: YTopCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYTopCoordinate = m_arrCoordinateRact[m_nChannelIndex].top;
	float fScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].top = int((nYTopCoordinate-wholeRect.left)*fScale);
	m_strLevelMapTop[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].top);
}

//yBottom坐标转yBottom灰度值
void CLevelWnd::YBottomCoordinateToGray()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYBottomCoordinate = m_arrCoordinateRact[m_nChannelIndex].bottom;
	float fScale = 255.0f/wholeRect.Width();
	m_arrLevelMapRect[m_nChannelIndex].bottom = int((nYBottomCoordinate-wholeRect.left)*fScale);
	m_strLevelMapBottom[m_nChannelIndex].Format(_T("%d"), m_arrLevelMapRect[m_nChannelIndex].bottom);
}

//灰度转坐标

//xLeft灰度值转xLeft坐标
void CLevelWnd::XLeftGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXLeftGray = m_arrLevelMapRect[m_nChannelIndex].left;
	if (m_fTranslateProportion[m_nChannelIndex]!=NULL)
	{
		float fScale = wholeRect.Width()/(m_fMaxMatrix[m_nChannelIndex]-m_fMinMatrix[m_nChannelIndex]+1);
		m_arrCoordinateRact[m_nChannelIndex].left =int((nXLeftGray-m_fMinMatrix[m_nChannelIndex])*fScale+wholeRect.left+0.5*fScale);
	}
	else
	{
		m_arrCoordinateRact[m_nChannelIndex].left = wholeRect.left;
	}
}

//xRight灰度值转xRight坐标
void CLevelWnd::XRightGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nXRightGray = m_arrLevelMapRect[m_nChannelIndex].right;
	if (m_fTranslateProportion[m_nChannelIndex] != NULL)
	{
		float fScale = wholeRect.Width()/(m_fMaxMatrix[m_nChannelIndex]-m_fMinMatrix[m_nChannelIndex]+1);
		m_arrCoordinateRact[m_nChannelIndex].right = int((nXRightGray-m_fMinMatrix[m_nChannelIndex])*fScale+wholeRect.left+0.5*fScale);
	}
	else
	{
		m_arrCoordinateRact[m_nChannelIndex].right = wholeRect.right;
	}
}

//拐点比例转拐点x坐标
void CLevelWnd::ProportionToXInflection()
{
	const CRect& coordinateRact = m_arrCoordinateRact[m_nChannelIndex];
	float& fProportion = m_fArrProportion[m_nChannelIndex];
	m_arrXInflection[m_nChannelIndex] = int(coordinateRact.Width()*fProportion/(1.0f+fProportion)+coordinateRact.left);
}

//yTop灰度值转yTop坐标
void CLevelWnd::YTopGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYTopGray = m_arrLevelMapRect[m_nChannelIndex].top;
	float ftScale = wholeRect.Width()/256.0f;
	m_arrCoordinateRact[m_nChannelIndex].top = int(nYTopGray*ftScale+wholeRect.left+0.5*ftScale);
}

//yBottom灰度值转yBottom坐标
void CLevelWnd::YBottomGrayToCoordinate()
{
	const CRect& wholeRect = m_Items.at(m_nHistogramIndex).m_Rect;
	const int& nYBottomGray = m_arrLevelMapRect[m_nChannelIndex].bottom;
	float ftScale = wholeRect.Width()/256.0f;
	m_arrCoordinateRact[m_nChannelIndex].bottom = int(nYBottomGray*ftScale+wholeRect.left+0.5*ftScale);
}

//更新坐标

//更新xLeft坐标
void CLevelWnd::XLeftChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].left;
	CRect& XLeftRect = m_Items.at(m_nSliderXLeftIndex).m_Rect;
	int nXLeftWidth = XLeftRect.Width();
	XLeftRect.left = LONG(nNewCenter-0.5*nXLeftWidth);
	XLeftRect.right = LONG(nNewCenter+0.5*nXLeftWidth);
	CWnd* pXLeft = m_Items.at(m_nSliderXLeftIndex).m_pItem;
	if (pXLeft != NULL)
	{
		pXLeft->MoveWindow(XLeftRect);
	}
}

//更新xRight坐标
void CLevelWnd::XRightChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].right;
	CRect& XRightRect = m_Items.at(m_nSliderXRightIndex).m_Rect;
	int nXRightWidth = XRightRect.Width();
	XRightRect.left = LONG(nNewCenter-0.5*nXRightWidth);
	XRightRect.right = LONG(nNewCenter+0.5*nXRightWidth);
	CWnd* pXRight = m_Items.at(m_nSliderXRightIndex).m_pItem;
	if (pXRight != NULL)
	{
		pXRight->MoveWindow(XRightRect);
	}
}

//更新拐点坐标
void CLevelWnd::XInflectionChangeCoordinate()
{
	int nNewCenter = m_arrXInflection[m_nChannelIndex];
	CRect& XInflectionRect = m_Items.at(m_nSliderXInflectionIndex).m_Rect;
	int nXInflectionWidth = XInflectionRect.Width();
	XInflectionRect.left = LONG(nNewCenter-0.5*nXInflectionWidth);
	XInflectionRect.right = LONG(nNewCenter+0.5*nXInflectionWidth);
	CWnd* pXInflection = m_Items.at(m_nSliderXInflectionIndex).m_pItem;
	if (pXInflection != NULL)
	{
		pXInflection->MoveWindow(XInflectionRect);
	}
}

//更新yTop坐标
void CLevelWnd::YTopChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].top;
	CRect& YTopRect = m_Items.at(m_nSliderYTopIndex).m_Rect;
	int nYTopWidth = YTopRect.Width();
	YTopRect.left = LONG(nNewCenter-0.5*nYTopWidth);
	YTopRect.right = LONG(nNewCenter+0.5*nYTopWidth);
	CWnd* pYTop = m_Items.at(m_nSliderYTopIndex).m_pItem;
	if (pYTop != NULL)
	{
		pYTop->MoveWindow(YTopRect);
	}
}

//更新yBottom坐标
void CLevelWnd::YBottomChangeCoordinate()
{
	int nNewCenter = m_arrCoordinateRact[m_nChannelIndex].bottom;
	CRect& YBottomRect = m_Items.at(m_nSliderYBottomIndex).m_Rect;
	int nYBottomWidth = YBottomRect.Width();
	YBottomRect.left = LONG(nNewCenter-0.5*nYBottomWidth);
	YBottomRect.right = LONG(nNewCenter+0.5*nYBottomWidth);
	CWnd* pYBottom= m_Items.at(m_nSliderYBottomIndex).m_pItem;
	if (pYBottom != NULL)
	{
		pYBottom->MoveWindow(YBottomRect);
	}
}

