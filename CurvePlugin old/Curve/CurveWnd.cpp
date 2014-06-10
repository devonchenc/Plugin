// AdjustCurveWnd.cpp : 实现文件
//

#include "stdafx.h"
#include "CurveDlg.h"
#include "CurveWnd.h"

#define BUTTON_WIDTH       40
#define BUTTON_HEIGHT      20

// CCurveWnd

IMPLEMENT_DYNAMIC(CCurveWnd, CWnd)

// 构造
CCurveWnd::CCurveWnd()
{
	for (int i = 0; i<4; i++)
	{
		m_pfHistogram[i] = NULL;
	}
	m_pComboBoxRGB = NULL;
	m_pBnGroupBox = NULL;
	m_pBnChooseCurve = NULL;
	m_pBnChooseLine = NULL;
	m_pBnReset = NULL;
	m_pBnInverse = NULL;
	m_pBnImportCurve = NULL;
	m_pBnExportCurve = NULL;
	m_pBnOK = NULL;
	m_pBnCancel = NULL;
	m_pXPosition = NULL;
	m_pYPosition = NULL;
	m_pXlabel = NULL;
	m_pYlabel = NULL;
	m_pOnPaintRect = NULL;
	m_nChannelSelected = CHANNEL_RGB;//默认通道：RGB
	m_bUserWillDrag = FALSE;
	m_posCurrentPeg = NULL;
	m_nLinePattern = LINE_PATTERN_SPLINE;
	m_bInverse = FALSE;
	//图像信息
	m_dataSrc = NULL;
	m_curData = NULL;
	m_nImageWidth = 0;
	m_nImageHeight = 0;
	m_nPixelQuantity = 0;
	m_nbytesWidth = 0;
	m_nChannel = 0;
	m_nlenData = 0;

	//OnSize
	m_pWndSize = NULL;

	//Font
	m_FontName = _T("Arial");
	m_nFontSize = 100;
}

// 析构
CCurveWnd::~CCurveWnd()
{
	for (int i = 0; i<4; i++)
	{
		if (m_pfHistogram[i] != NULL)
		{
			delete [] m_pfHistogram[i];
		}

		m_V4Transform[i].~vector();

		POSITION pos = m_pPegsList[i].GetHeadPosition();
		while (pos != NULL)
		{
			CPoint *Temp = m_pPegsList[i].GetNext(pos).m_pt;
			delete Temp;
		}
		m_pPegsList[i].RemoveAll();
	}
	if (m_pComboBoxRGB != NULL)  {	delete m_pComboBoxRGB;}
	if (m_pBnGroupBox != NULL)   {	delete m_pBnGroupBox;}
	if (m_pBnChooseCurve != NULL){	delete m_pBnChooseCurve;}
	if (m_pBnChooseLine != NULL) {	delete m_pBnChooseLine;}
	if (m_pBnReset != NULL)      {	delete m_pBnReset;}
	if (m_pBnInverse != NULL)    {	delete m_pBnInverse;}
	if (m_pBnImportCurve != NULL){	delete m_pBnImportCurve;}
	if (m_pBnExportCurve != NULL){	delete m_pBnExportCurve;}
	if (m_pBnOK != NULL)         {	delete m_pBnOK;}
	if (m_pBnCancel != NULL)     {	delete m_pBnCancel;}
	if (m_pXPosition != NULL)    {	delete m_pXPosition;}
	if (m_pYPosition != NULL)    {	delete m_pYPosition;}
	if (m_pXlabel != NULL)       {	delete m_pXlabel;}
	if (m_pYlabel != NULL)       {	delete m_pYlabel;}
	if (m_pOnPaintRect != NULL)  {	delete m_pOnPaintRect;}
	if (m_dataSrc != NULL)       {  delete [] m_dataSrc;}
	if (m_curData != NULL)       {  delete [] m_curData;}
	if (m_pWndSize != NULL)      {  delete m_pWndSize;}
	if (m_pAppFont != NULL)      {	delete m_pAppFont;}
}

BEGIN_MESSAGE_MAP(CCurveWnd, CWnd)
	//创建、绘制消息映射
	ON_WM_CREATE()
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	//鼠标消息映射
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONUP()
	//按钮消息映射
	ON_CBN_SELCHANGE(IDC_COMBOBOX_RGB, &CCurveWnd::OnComboboxRgb)
	ON_BN_CLICKED(IDB_CHOOSE_CURVE, OnChooseCurve)
	ON_BN_CLICKED(IDB_CHOOSE_LINE, &CCurveWnd::OnChooseLine)
	ON_BN_CLICKED(IDB_CURVEWND_RESET, &CCurveWnd::OnIdbCurvewndReset)
	ON_BN_CLICKED(IDB_CURVEWND_INVERSE, &CCurveWnd::OnIdbCurvewndInverse)
	ON_BN_CLICKED(IDB_IMPORT_CURVE, &CCurveWnd::OnIdbImportCurve)
	ON_BN_CLICKED(IDB_EXPORT_CURVE, &CCurveWnd::OnIdbExportCurve)
	ON_BN_CLICKED(IDB_CURVE_OK, &CCurveWnd::OnIdbCurveOk)
	ON_BN_CLICKED(IDB_CURVE_CANCEL, &CCurveWnd::OnIdbCurveCancel)
END_MESSAGE_MAP()

// CCurveWnd 消息处理程序

int CCurveWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_pAppFont = new CFont;
	m_pAppFont->CreatePointFont(m_nFontSize, m_FontName);

	m_pWndSize = new CRect;
	GetClientRect(m_pWndSize);

	int nButtonPosX1 = m_pWndSize->right-50;
	int nButtonPosX2 = nButtonPosX1+BUTTON_WIDTH;
	double Space = m_pWndSize->Height()/9.0;

	//绘图区Rect
	m_pOnPaintRect = new CRect(40, 40, m_pWndSize->right-70, m_pWndSize->bottom-80);

	//宽高相对于255缩放比
	m_nRectWidth = m_pOnPaintRect->Width();
	m_nRectHeight = m_pOnPaintRect->Height();
	m_fWidthRatio = m_nRectWidth/255.0;
	m_fHeightRatio = m_nRectHeight/255.0;

	//Group Box
	m_pBnGroupBox = new CButton;
	m_pBnGroupBox->Create(_T("通道"), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, CRect(20, 20, m_pWndSize->right-60, m_pWndSize->bottom-60), this, IDB_GROUP_BOX);
	m_pBnGroupBox->SetFont(m_pAppFont, TRUE);

	//通道:RGB
	m_pComboBoxRGB = new CComboBox;
	m_pComboBoxRGB->Create(WS_CHILD | WS_VISIBLE | CBS_AUTOHSCROLL | CBS_DROPDOWNLIST | WS_EX_TOPMOST, CRect(70, 15, 130, 35), this, IDC_COMBOBOX_RGB);
	m_pComboBoxRGB->AddString(_T("RGB"));
	m_pComboBoxRGB->AddString(_T("红"));
	m_pComboBoxRGB->AddString(_T("绿"));
	m_pComboBoxRGB->AddString(_T("蓝"));
	m_pComboBoxRGB->SetCurSel(0);
	m_pComboBoxRGB->SetFont(m_pAppFont, TRUE);

	//选择曲线
	m_pBnChooseCurve = new CMFCButton;
	m_pBnChooseCurve->Create(_T("曲线"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nButtonPosX1, int(Space), nButtonPosX2, int(Space+BUTTON_HEIGHT)), this, IDB_CHOOSE_CURVE);
	m_pBnChooseCurve->SetFont(m_pAppFont, TRUE);
	m_pBnChooseCurve->SetTooltip(_T("三次样条插值"));

	//选择直线
	m_pBnChooseLine = new CMFCButton;
	m_pBnChooseLine->Create(_T("直线"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nButtonPosX1, int(2*Space), nButtonPosX2, int(2*Space+BUTTON_HEIGHT)), this, IDB_CHOOSE_LINE);
	m_pBnChooseLine->SetFont(m_pAppFont, TRUE);
	m_pBnChooseLine->SetTooltip(_T("分段线性插值"));

	//重置
	m_pBnReset = new CMFCButton;
	m_pBnReset->Create(_T("重置"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nButtonPosX1, int(3*Space), nButtonPosX2, int(3*Space+BUTTON_HEIGHT)), this, IDB_CURVEWND_RESET);
	m_pBnReset->SetFont(m_pAppFont, TRUE);
	m_pBnReset->SetTooltip(_T("撤销所有操作"));

	//反转
	m_pBnInverse = new CMFCButton;
	m_pBnInverse->Create(_T("反转"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nButtonPosX1, int(4*Space), nButtonPosX2, int(4*Space+BUTTON_HEIGHT)), this, IDB_CURVEWND_INVERSE);
	m_pBnInverse->SetFont(m_pAppFont, TRUE);
	m_pBnInverse->SetTooltip(_T("反转图像"));

	//导入曲线
	m_pBnImportCurve = new CMFCButton;
	m_pBnImportCurve->Create(_T("导入"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nButtonPosX1, int(5*Space), nButtonPosX2, int(5*Space+BUTTON_HEIGHT)), this, IDB_IMPORT_CURVE);
	m_pBnImportCurve->SetFont(m_pAppFont, TRUE);
	m_pBnImportCurve->SetTooltip(_T("导入已有曲线"));

	//导出曲线
	m_pBnExportCurve = new CMFCButton;
	m_pBnExportCurve->Create(_T("导出"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(nButtonPosX1, int(6*Space), nButtonPosX2, int(6*Space+BUTTON_HEIGHT)), this, IDB_EXPORT_CURVE);
	m_pBnExportCurve->SetFont(m_pAppFont, TRUE);
	m_pBnExportCurve->SetTooltip(_T("导出变换曲线"));

	//X坐标
	m_pXPosition = new CEdit;
	m_pXPosition->Create(WS_CHILD | WS_VISIBLE, CRect(nButtonPosX1+20, int(7*Space), nButtonPosX2, int(7*Space+BUTTON_HEIGHT)), this, IDE_CURVEWND_XPOSITION);
	m_pXPosition->SetFont(m_pAppFont, TRUE);

	//Y坐标
	m_pYPosition = new CEdit;
	m_pYPosition->Create(WS_CHILD | WS_VISIBLE, CRect(nButtonPosX1+20, int(8*Space), nButtonPosX2, int(8*Space+BUTTON_HEIGHT)), this, IDE_CURVEWND_YPOSITION);
	m_pYPosition->SetFont(m_pAppFont, TRUE);

	//X标签
	m_pXlabel = new CStatic;
	m_pXlabel->Create(_T("X:"), WS_CHILD | WS_VISIBLE, CRect(nButtonPosX1, int(7*Space), nButtonPosX2-30, int(7*Space+BUTTON_HEIGHT)), this, IDS_CURVEWND_XLABEL);
	m_pXlabel->SetFont(m_pAppFont, TRUE);

	//Y标签
	m_pYlabel = new CStatic;
	m_pYlabel->Create(_T("Y:"), WS_CHILD | WS_VISIBLE, CRect(nButtonPosX1, int(8*Space), nButtonPosX2-30, int(8*Space+BUTTON_HEIGHT)), this, IDS_CURVEWND_YLABEL);
	m_pYlabel->SetFont(m_pAppFont, TRUE);

	//OK
	m_pBnOK = new CMFCButton;
	m_pBnOK->Create(_T("确定"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(40, m_pWndSize->bottom-40, 40+BUTTON_WIDTH, m_pWndSize->bottom-40+BUTTON_HEIGHT), this, IDB_CURVE_OK);
	m_pBnOK->SetFont(m_pAppFont, TRUE);
	m_pBnOK->SetTooltip(_T("保存修改"));

	//CANCEL
	m_pBnCancel = new CMFCButton;
	m_pBnCancel->Create(_T("取消"), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, CRect(m_pWndSize->right-70-BUTTON_WIDTH, m_pWndSize->bottom-40, m_pWndSize->right-70, m_pWndSize->bottom-40+BUTTON_HEIGHT), this, IDB_CURVE_CANCEL);
	m_pBnCancel->SetFont(m_pAppFont, TRUE);
	m_pBnCancel->SetTooltip(_T("取消修改"));

	//*** m_pImage ***//
	m_pImage = GetCurImage();

	//初始化直方图
	GetHistogram();

	return 0;
}

//
// *** 函数 ***
//

CDocument* CCurveWnd::GetDocument()
{
	return PIGetActiveDocument();
}

//获取图像指针
CImage* CCurveWnd::GetCurImage()
{
	CDocument* pDoc = PIGetActiveDocument();
	if (pDoc == NULL)
		return NULL;
	if (!pDoc->IsKindOf(RUNTIME_CLASS(CPIDocument)))
		return NULL;

	CImage* pImage = ((CPIDocument*)pDoc)->GetImage();
	return pImage;
}

//获取直方图
BOOL CCurveWnd::GetHistogram()
{
	if (m_pImage == NULL) return FALSE;

	//初始化pegs
	for (int i = 0; i<4; i++)
	{
		peg pBegin;
		pBegin.m_dRatioX = 0;
		pBegin.m_dRatioY = 1;
		pBegin.m_pt = new CPoint(m_pOnPaintRect->left, m_pOnPaintRect->bottom);
		
		peg pEnd;
		pEnd.m_dRatioX = 1;
		pEnd.m_dRatioY = 0;
		pEnd.m_pt = new CPoint(m_pOnPaintRect->right, m_pOnPaintRect->top);
		m_pPegsList[i].AddTail(pBegin);
		m_pPegsList[i].AddTail(pEnd);
	}

	//获取图像信息
	m_nImageWidth = m_pImage->GetWidth();
	m_nImageHeight = m_pImage->GetHeight();
	m_nPixelQuantity = m_nImageWidth*m_nImageHeight;
	m_nChannel = m_pImage->GetBPP()/8;
	m_nbytesWidth = m_nChannel*m_nImageWidth;
	m_nlenData = m_nbytesWidth*m_nImageHeight;
	//将图像源数据储存起来
	m_dataSrc = new BYTE[m_nlenData];
	m_curData = new BYTE[m_nlenData];
	m_pBits = (BYTE *)m_pImage->GetBits()+(m_pImage->GetPitch()*(m_nImageHeight-1));//获得图像数据首地址
	memcpy(m_dataSrc, m_pBits, m_nlenData);
	memcpy(m_curData, m_pBits, m_nlenData);

	//初始化直方图
	for (int i = 0; i<4; i++)
	{
		m_pfHistogram[i] = new double[256];
		for (int j = 0; j<256; j++)
		{
			m_pfHistogram[i][j] = 0;
		}
	}

	//计算直方图
	BYTE *Pixel = m_dataSrc;
	for (int i = 0; i<m_nImageHeight; i++)
	{
		for (int j = 0; j<m_nImageWidth; j++)
		{//RGB24各分量的排列顺序为：BGR, RGB32各分量的排列顺序为：BGRA
			m_pfHistogram[3][int(*Pixel)]++;                                              //Blue
			m_pfHistogram[2][int(*(Pixel+1))]++;                                          //Green
			m_pfHistogram[1][int(*(Pixel+2))]++;                                          //Red
			if (*Pixel == *(Pixel+1) && *(Pixel+1) == *(Pixel+2))
			{//此处解决直方图中某些灰度缺失的问题,2014.4.14
				m_pfHistogram[0][int(*Pixel)]++;                                              //RGB
			}
			else
			{
				m_pfHistogram[0][int(*Pixel*0.299 + *(Pixel+1)*0.587 + *(Pixel+2)*0.114)]++;  //RGB
			}
			Pixel += m_nChannel;
		}
	}

	//归一化
	for (int i = 0, j; i<4; i++)
	{
		for (j = 0; j<256; j++)
		{
			m_pfHistogram[i][j] = m_pfHistogram[i][j]/m_nPixelQuantity;
		}
	}
	return TRUE;
}

//看是否已存在,若存在则要替换
BOOL CCurveWnd::PegIsExist(CPoint& point)
{
	//看是否已存在
	POSITION prev, pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	double errorX = max(m_nRectWidth/100, 3.0), errorY = max(m_nRectHeight/100, 3.0);
	while (pos != NULL)
	{
		prev = pos;
		peg& Temp = m_pPegsList[m_nChannelSelected].GetNext(pos);
		double err = fabs( 1.0*point.x - Temp.m_pt->x );//x坐标之差
		if (Temp.m_pt->x == m_pOnPaintRect->left || Temp.m_pt->x == m_pOnPaintRect->right)
		{//对于边界点：边界点的x坐标不能被修改
			if (err<errorX)
			{
				m_posCurrentPeg = prev;
				Temp.m_pt->y = point.y;
				Temp.m_dRatioY = 1.0*(point.y-m_pOnPaintRect->top)/m_nRectHeight;
				break;
			}
		}
		else if (err == 0) //x坐标相等,此处有问题。这可能导致图像画不出来。2014.4.23备注
		{
			return -1;
		}
		else if ( err < errorX && fabs( 1.0*point.y - Temp.m_pt->y ) < errorY)
		{//对于非边界点：两个pegs之间间隔很小，便不能新增peg,此时替换peg
			m_posCurrentPeg = prev;
			*(Temp.m_pt) = point;
			Temp.m_dRatioX = 1.0*(point.x-m_pOnPaintRect->left)/m_nRectWidth;
			Temp.m_dRatioY = 1.0*(point.y-m_pOnPaintRect->top)/m_nRectHeight;
			break;
		}
	}
	if (pos != NULL)
	{//peg已经存在
		return TRUE;
	}
	return FALSE;
}

//增加一个新的peg
POSITION CCurveWnd::AddNewPeg(CPoint& point)
{
	//请先判断peg不存在
	POSITION prev, pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	while (pos != NULL)
	{
		prev = pos;
		peg& Temp = m_pPegsList[m_nChannelSelected].GetNext(pos);
		if (point.x < Temp.m_pt->x )
		{
			//将peg插到Temp前面
			peg NewPoint;
			NewPoint.m_pt = new CPoint(point);
			NewPoint.m_dRatioX = 1.0*(point.x-m_pOnPaintRect->left)/m_nRectWidth;
			NewPoint.m_dRatioY = 1.0*(point.y-m_pOnPaintRect->top)/m_nRectHeight;
			m_pPegsList[m_nChannelSelected].InsertBefore(prev, NewPoint);
			m_pPegsList[m_nChannelSelected].GetPrev(prev);//新增peg的位置
			m_posCurrentPeg = prev;
			return prev;
		}
	}
	return NULL;
}

//用户想改变某个peg，表现为点击鼠标的位置离某个peg很接近
BOOL CCurveWnd::RePlacePeg(CPoint& point)
{
	POSITION prev, pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	while (pos != NULL)
	{
		prev = pos;
		peg& Temp = m_pPegsList[m_nChannelSelected].GetNext(pos);
		if (fabs(1.0*point.x - Temp.m_pt->x)<max(m_nRectWidth/100, 3.0) || fabs(1.0*point.y - Temp.m_pt->y)<max(m_nRectHeight/100, 3.0))
		{
			//将prev位置的peg删掉
			if (Temp.m_pt->x != m_pOnPaintRect->left && Temp.m_pt->x != m_pOnPaintRect->right)
			{//边界点不能被删除
				delete m_pPegsList[m_nChannelSelected].GetAt(prev).m_pt;
				m_pPegsList[m_nChannelSelected].RemoveAt(prev);
			}
			return TRUE;
		}
	}
	return FALSE;
}

//看样子用户想增加一个peg
BOOL CCurveWnd::UserWillDrag(CPoint& point)
{
	int error = point.y - m_V4Transform[m_nChannelSelected][point.x - m_pOnPaintRect->left];
	if (error < max(m_nRectHeight/100, 16)) return TRUE;
	return FALSE;
}

//样条函数:根据m_pPegList插值出[0,255]的灰度变换
void CCurveWnd::SplineFunc()
{
    //原型: VECTOR Spline(VECTOR Node, VECTOR Value, VECTOR x0, double c1, double c2, int CASE = 2)
	if (m_pImage == NULL) return;
	VECTOR Node, Value, x0, y0;
	POSITION pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	while (pos != NULL)
	{
		CPoint *Temp = m_pPegsList[m_nChannelSelected].GetNext(pos).m_pt;
		Node.push_back(double(Temp->x));
		Value.push_back(double(Temp->y));
	}
	for (int i = m_pOnPaintRect->left; i<=m_pOnPaintRect->right; i++)
	{
		x0.push_back(double(i));
	}
	y0 = Spline(Node, Value, x0, 0, 0);
	for (int i = m_pOnPaintRect->left, j = 0; i<=m_pOnPaintRect->right; i++, j++)
	{
		m_V4Transform[m_nChannelSelected][j] = int(y0[j]);
	}
	Threshold();
	Node.~vector();
	Value.~vector();
	x0.~vector();
	y0.~vector();
	return;
}

//分段线性函数:根据m_pPegList插值出[0,255]的灰度变换
void CCurveWnd::PieceWiseFunc()
{
	if (m_pImage == NULL) return;
	POSITION pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	while (pos != NULL)
	{
		CPoint *Temp = m_pPegsList[m_nChannelSelected].GetNext(pos).m_pt;
		if (pos != NULL)
		{
			CPoint *Next = m_pPegsList[m_nChannelSelected].GetAt(pos).m_pt;
			double Slope = double(Temp->y - Next->y)/(Temp->x - Next->x);
			for (int i = Temp->x; i<=Next->x; i++)
			{
				m_V4Transform[m_nChannelSelected][i-m_pOnPaintRect->left] = long(Slope*(i-Temp->x) + Temp->y);
			}
		}
		else
		{
			break;
		}
	}
}

//光标在绘图区域之内
BOOL CCurveWnd::CursorIsInPaintRect(CPoint& point)
{
	if (point.x >= m_pOnPaintRect->left && point.x <= m_pOnPaintRect->right && point.y >= m_pOnPaintRect->top && point.y <= m_pOnPaintRect->bottom)
		return TRUE;
	return FALSE;
}

//转换坐标，坐标原点为Dialog的左上角顶点
CPoint CCurveWnd::TransPositionToDialog(CPoint& point)
{
	return CPoint(point.x + m_pOnPaintRect->left, m_pOnPaintRect->bottom - point.y);
}

//转换坐标，坐标原点为Rect的左下角顶点
CPoint CCurveWnd::TransPositionToRectangle(CPoint& point)
{
	return CPoint(point.x - m_pOnPaintRect->left, m_pOnPaintRect->bottom - point.y);
}

//根据插值出来的灰度变换曲线作灰度变换
BOOL CCurveWnd::GrayTransform()
{
	if (m_pImage == NULL) return FALSE;

	//每次灰度变换时，需要重新载入图像源数据
	//防止每次灰度变换的效果叠加产生不良效果
	memcpy(m_curData, m_dataSrc, m_nlenData);
	//直方图预操作，使用后必须释放TempCell的内存
	int* TempCell = PreTransform();
	BYTE* Pixel = m_curData;
	if (m_nChannelSelected == CHANNEL_RGB)
	{
		//RGB通道
		for (int i = 0; i<m_nImageHeight; i++)
		{
			for (int j = 0; j<m_nImageWidth; j++)
			{
				for (int k = 0; k<m_nChannel; k++)
				{
					//解决内存溢出错误,2014.4.17
					*(Pixel+k) = TempCell[int(*(Pixel+k))];
				}
				Pixel += m_nChannel;
			}
		}
	}
	else
	{
		//其他通道
		for (int i = 0; i<m_nImageHeight; i++)
		{
			for (int j = 0; j<m_nImageWidth; j++)
			{
				*(Pixel+3-m_nChannelSelected) = TempCell[int(*(Pixel+3-m_nChannelSelected))];
				Pixel += m_nChannel;
			}
		}
	}
	//修改图像
	memcpy(m_pBits, m_curData, m_nlenData);
	delete [] TempCell;
	return TRUE;
}

//对超出Rect的变换值进行处理
void CCurveWnd::Threshold()
{
	for (int i = 0; i<m_nRectWidth+1; i++)
	{
		if(m_V4Transform[m_nChannelSelected][i]<m_pOnPaintRect->top)
		{
			m_V4Transform[m_nChannelSelected][i] = m_pOnPaintRect->top;
			continue;
		}
		if(m_V4Transform[m_nChannelSelected][i]>m_pOnPaintRect->bottom)
		{//bottom-1退而求其次，解决top和bottom绘制不出直线的问题.2014.4.15
			m_V4Transform[m_nChannelSelected][i] = m_pOnPaintRect->bottom-1;
			continue;
		}
	}
}

//对m_pnTransform预操作,计算了256个数据，对应于0到255的灰度变换值
//使用后必须使用delete释放内存
int* CCurveWnd::PreTransform()
{
	int *Result = new int[256];
	for (int j = 0; j<256; j++)
	{
		Result[j] = int ((m_pOnPaintRect->bottom - m_V4Transform[m_nChannelSelected][int(j*m_fWidthRatio)])/m_fHeightRatio);
	}
	return Result;
}

//图像反转
void CCurveWnd::InverseTransform()
{
	POSITION pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	while (pos != NULL)
	{
		peg& Temp = m_pPegsList[m_nChannelSelected].GetNext(pos);
		int gray = m_pOnPaintRect->bottom - Temp.m_pt->y;
		Temp.m_pt->y = m_pOnPaintRect->top + gray;
		Temp.m_dRatioY = 1.0*gray/m_nRectHeight;
	}
}

//立即刷新图像:先对图像进行灰度变换，之后把当前状态存入栈
void CCurveWnd::ImshowImmediately()
{
	if(GrayTransform() == FALSE) return;

	CDocument* pDoc = GetDocument();
	memcpy(m_pBits, m_curData, m_nlenData);
	PIGetActiveView()->Invalidate(FALSE);
	//pDoc->UpdateWindow();
}

//先根据pegs做插值，然后绘制变换曲线
void CCurveWnd::ShowGrayTransform()
{
	if (m_pImage == NULL) return;
	switch (m_nLinePattern)
	{//看要画什么曲线
	case LINE_PATTERN_SPLINE:
		SplineFunc();
		break;
	case LINE_PATTERN_LINEAR:
		PieceWiseFunc();
		break;
	default:
		SplineFunc();
		break;
	}
	InvalidateRect(m_pOnPaintRect);
}

//保存当前状态：比如切换通道时候保存图像
void CCurveWnd::SaveCurrentState()
{
	if (m_pImage == NULL) return;

	memcpy(m_dataSrc, m_curData, m_nlenData);
}

//
//*** 响应 ***
//

//绘制
void CCurveWnd::OnPaint()
{
	//在指定区域rect里面绘制直方图
	CPaintDC dc(this);
	CBitmap MemBitmap;
	CDC MemDC;
	CRect WndSize;
	GetClientRect(&WndSize);
	MemDC.CreateCompatibleDC(&dc);
	MemBitmap.CreateCompatibleBitmap(&dc, WndSize.Width(), WndSize.Height());

	CBitmap* pOldBmp = MemDC.SelectObject(&MemBitmap);
	MemDC.FillSolidRect(m_pOnPaintRect, RGB(240, 240, 240));
	CPen *pOldPen, newPen;
	COLORREF LineColor;

	//水平方向颜色条
	for (int i = m_pOnPaintRect->left, k = 0; i<m_pOnPaintRect->right; i++, k++)
	{
		LineColor = ColorSetting(m_nChannelSelected, int(k/m_fWidthRatio), 1);
		newPen.CreatePen(PS_SOLID, 1, LineColor);
		pOldPen = MemDC.SelectObject(&newPen);
		MemDC.MoveTo(i, m_pOnPaintRect->bottom + 5);
		MemDC.LineTo(i, m_pOnPaintRect->bottom + 10);
		MemDC.SelectObject(pOldPen);
		newPen.DeleteObject();
	}
	//垂直方向颜色条
	for (int j = m_pOnPaintRect->top, k = 0; j<m_pOnPaintRect->bottom; j++, k++)
	{

		LineColor = ColorSetting(m_nChannelSelected, int(k/m_fHeightRatio), 1);
		newPen.CreatePen(PS_SOLID, 1, LineColor);
		pOldPen = MemDC.SelectObject(&newPen);
		MemDC.MoveTo(m_pOnPaintRect->left - 10, m_pOnPaintRect->bottom-j);
		MemDC.LineTo(m_pOnPaintRect->left - 5, m_pOnPaintRect->bottom-j);
		MemDC.SelectObject(pOldPen);
		newPen.DeleteObject();
	}

	if (m_pImage != NULL)
	{
		//直方图
		LineColor = ColorSetting(m_nChannelSelected, 128);
		newPen.CreatePen(PS_SOLID, 1, LineColor);
		pOldPen = MemDC.SelectObject(&newPen);
		double rate = 0.20*m_nRectHeight*m_nRectWidth/m_fWidthRatio;//使直方图尽量布满坐标系并且便于观察
		for (int i = 0, nTemp; i<m_nRectWidth; i++)
		{
			nTemp = m_pOnPaintRect->left+i;
			MemDC.MoveTo(nTemp, m_pOnPaintRect->bottom);
			MemDC.LineTo(nTemp, int(m_pOnPaintRect->bottom - m_pfHistogram[m_nChannelSelected][int(i/m_fWidthRatio)]*rate));
		}
		newPen.DeleteObject();

		//变换曲线
		LineColor = ColorSetting(m_nChannelSelected, 255);
		newPen.CreatePen(PS_SOLID, 1, LineColor);
		pOldPen = MemDC.SelectObject(&newPen);
		for (int i = 1; i<=m_nRectWidth; i++)
		{
			MemDC.MoveTo(m_pOnPaintRect->left + i-1, m_V4Transform[m_nChannelSelected][i-1]);
			MemDC.LineTo(m_pOnPaintRect->left + i, m_V4Transform[m_nChannelSelected][i]);
		}
		MemDC.SelectObject(pOldPen);
		newPen.DeleteObject();

		//pegs
		CBrush *pOldBrush, newBrush;
		newPen.CreatePen(PS_SOLID, 1, RGB(0, 128, 128));
		pOldPen = MemDC.SelectObject(&newPen);
		POSITION pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
		while (pos != NULL)
		{
			if (pos != m_posCurrentPeg)
			{//非当前拽动的peg
				CPoint *Temp = m_pPegsList[m_nChannelSelected].GetNext(pos).m_pt;
				newBrush.CreateSolidBrush(RGB(0, 128, 128));
				pOldBrush = MemDC.SelectObject(&newBrush);
				MemDC.Rectangle(CRect(CPoint(Temp->x - 3, Temp->y - 3), CPoint(Temp->x + 3, Temp->y + 3)));
				MemDC.SelectObject(pOldBrush);
				newBrush.DeleteObject();
			}
			else
			{//当前拽动的peg
				CPoint *Temp = m_pPegsList[m_nChannelSelected].GetNext(pos).m_pt;
				newBrush.CreateSolidBrush(RGB(255, 0, 0));
				pOldBrush = MemDC.SelectObject(&newBrush);
				MemDC.Rectangle(CRect(CPoint(Temp->x - 3, Temp->y - 3), CPoint(Temp->x + 3, Temp->y + 3)));
				MemDC.SelectObject(pOldBrush);
				newBrush.DeleteObject();
			}
		}
		MemDC.SelectObject(pOldPen);
		newPen.DeleteObject();
	}

	//对角线,应该画在直方图后面，以免被覆盖
	newPen.CreatePen(PS_SOLID, 1, RGB(100, 100, 100));
	pOldPen = MemDC.SelectObject(&newPen);
	if (m_bInverse)
	{//图像已反转,对角线要变化,2014.4.15
		MemDC.MoveTo(m_pOnPaintRect->left, m_pOnPaintRect->top);
		MemDC.LineTo(m_pOnPaintRect->right, m_pOnPaintRect->bottom);
	}
	else
	{
		MemDC.MoveTo(m_pOnPaintRect->left, m_pOnPaintRect->bottom);
		MemDC.LineTo(m_pOnPaintRect->right, m_pOnPaintRect->top);
	}
	MemDC.SelectObject(pOldPen);
	newPen.DeleteObject();

	//拷贝
	dc.BitBlt(m_pOnPaintRect->left, m_pOnPaintRect->top, m_nRectWidth, m_nRectHeight, &MemDC, m_pOnPaintRect->left, m_pOnPaintRect->top, SRCCOPY);
	dc.BitBlt(m_pOnPaintRect->left - 10, m_pOnPaintRect->top, 5, m_nRectHeight, &MemDC, m_pOnPaintRect->left - 10, m_pOnPaintRect->top, SRCCOPY);    //垂直 color bar
	dc.BitBlt(m_pOnPaintRect->left, m_pOnPaintRect->bottom + 5, m_nRectWidth, 5, &MemDC, m_pOnPaintRect->left, m_pOnPaintRect->bottom + 5, SRCCOPY); //水平 color bar
	MemDC.SelectObject(pOldBmp);
	pOldBmp->DeleteObject();
	MemBitmap.DeleteObject();
}

void CCurveWnd::OnChooseCurve()
{
	m_nLinePattern = LINE_PATTERN_SPLINE;
	SplineFunc();
	GrayTransform();
	/*刷新视图*/
	InvalidateRect(m_pOnPaintRect);
	m_pBnChooseCurve->EnableWindow(FALSE);
	m_pBnChooseLine->EnableWindow(TRUE);
}

void CCurveWnd::OnChooseLine()
{
	m_nLinePattern = LINE_PATTERN_LINEAR;
	PieceWiseFunc();
	GrayTransform();
	/*刷新视图*/
	InvalidateRect(m_pOnPaintRect);
	m_pBnChooseCurve->EnableWindow(TRUE);
	m_pBnChooseLine->EnableWindow(FALSE);
}

void CCurveWnd::OnComboboxRgb()
{
	// TODO: 在此添加命令处理程序代码
	m_nChannelSelected = m_pComboBoxRGB->GetCurSel();
	SaveCurrentState();
	//修改peg的坐标
	ReshapePegs();
	//修改灰度变换函数
	ReshapeTransform();
	// 颜色条也更新
	CRect rect = CRect(m_pOnPaintRect->left - 10, m_pOnPaintRect->top, m_pOnPaintRect->right, m_pOnPaintRect->bottom + 10);
	InvalidateRect(rect);
	//（1）下面解决导入曲线后切换通道不画变换函数的问题,2014.4.14
	ShowGrayTransform();
	//GrayTransform();//（2）导致直方图出现“断裂带”.2014.6.11
	//摒弃该方案，尝试新的方案解决问题（1）
}

void CCurveWnd::OnIdbCurvewndReset()
{
	// TODO: 在此添加命令处理程序代码
	// 重置pegs
	for (int i = 0; i<4; i++)
	{
		POSITION pos = m_pPegsList[i].GetHeadPosition();
		while (pos != NULL)
		{
			CPoint* pt = m_pPegsList[i].GetNext(pos).m_pt;
			delete pt;
		}
		m_pPegsList[i].RemoveAll();
		peg pBegin;
		pBegin.m_pt = new CPoint(m_pOnPaintRect->left, m_pOnPaintRect->bottom);
		pBegin.m_dRatioX = 0;
		pBegin.m_dRatioY = 1;
		peg pEnd;
		pEnd.m_pt = new CPoint(m_pOnPaintRect->right, m_pOnPaintRect->top);
		pEnd.m_dRatioX = 1;
		pEnd.m_dRatioY = 0;
		m_pPegsList[i].AddTail(pBegin);
		m_pPegsList[i].AddTail(pEnd);
	}
	//重置变换曲线
	ShowGrayTransform();
	//一些变量重置
	m_bInverse = FALSE;
	//更新视图
	ImshowImmediately();//解决重置失效问题,2014.4.15
}

void CCurveWnd::OnIdbCurvewndInverse()
{
	// TODO: 在此添加命令处理程序代码
	m_bInverse = !m_bInverse;
	InverseTransform();
	ShowGrayTransform();
	ImshowImmediately();//解决翻转失效问题,2014.4.15
}

void CCurveWnd::OnIdbImportCurve()
{
	if (m_pImage == NULL) return;

	CString FilePath = _T("");
	CFileDialog hFileDlg(TRUE, NULL, NULL, OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST | OFN_READONLY, TEXT("curve(*.cv)|*.cv||"), NULL);
	hFileDlg.m_ofn.hwndOwner = m_hWnd;
	hFileDlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	hFileDlg.m_ofn.lpstrTitle = TEXT("打开曲线文件...\0");
	hFileDlg.m_ofn.nMaxFile = MAX_PATH;
	if(hFileDlg.DoModal() == IDOK)
	{
		FilePath = hFileDlg.GetPathName();
	}
	else return;

	CFile file;
	if(!file.Open(FilePath, CFile::modeRead))
	{
		MessageBox(_T("打开文件")+FilePath+_T("失败!"));
		return ;
	}
	CArchive ar(&file, CArchive::load);
	//重置pegs
	for (int i = 0, PegsQuantity; i<4; i++)
	{
		POSITION pos = m_pPegsList[i].GetHeadPosition();
		while (pos != NULL)
		{
			CPoint* pt = m_pPegsList[i].GetNext(pos).m_pt;
			delete pt;
		}
		m_pPegsList[i].RemoveAll();
		ar>>PegsQuantity;
		for(int j = 0; j<PegsQuantity; j++)
		{
			peg Temp;
			Temp.m_pt = new CPoint;
			ar>>*(Temp.m_pt);
			ar>>Temp.m_dRatioX;
			ar>>Temp.m_dRatioY;
			m_pPegsList[i].AddTail(Temp);
		}
	}
	ar.Close();
	file.Close();
	//变换并刷新视图
	int Swap = m_nChannelSelected;
	for (int i = 0; i<4; i++)
	{   //在这里解决导入曲线之后切换通道显示不正常的问题
		//也解决直方图出现"断裂带"的问题(因切换通道引起)
		//即 2014.4.14 和 2014.6.11 查出的漏洞.
		m_nChannelSelected = i;
		ReshapeTransform();
		ShowGrayTransform();
		GrayTransform();
		SaveCurrentState();
	}
	m_nChannelSelected = Swap;
	ImshowImmediately();
}

void CCurveWnd::OnIdbExportCurve()
{
	if (m_pImage == NULL) return;

	SYSTEMTIME sys;
	GetLocalTime(&sys);
	CString CurTime;
	CurTime.Format(_T("%04d%02d%02d%02d%02d%02d"), sys.wYear, sys.wMonth, sys.wDay, sys.wHour, sys.wMinute, sys.wSecond);
	CString FilePath = _T("");

	CFileDialog dlg(FALSE, NULL, CurTime, NULL, TEXT("curve(*.cv)|*.cv||"), NULL);
	dlg.m_ofn.hwndOwner = m_hWnd;
	dlg.m_ofn.lStructSize = sizeof(OPENFILENAME);
	dlg.m_ofn.lpstrTitle = TEXT("保存曲线...\0");
	dlg.m_ofn.nMaxFile = MAX_PATH;

	if ( dlg.DoModal() != IDOK)
		return;
	else
	{
		FilePath += dlg.GetPathName();
		if (FilePath.Right(3) != _T(".cv"))
		{
			FilePath += _T(".cv");
		}
	}

	CFile file;
	file.Open(FilePath, CFile::modeCreate | CFile::modeWrite);
	CArchive ar(&file, CArchive::store);
	for (int i = 0; i<4; i++)
	{
		POSITION pos = m_pPegsList[i].GetHeadPosition();
		ar<<m_pPegsList[i].GetSize();
		while (pos != NULL)
		{
			peg& Temp = m_pPegsList[i].GetNext(pos);
			ar<<*(Temp.m_pt);
			ar<<Temp.m_dRatioX;
			ar<<Temp.m_dRatioY;
		}
	}
	ar.Close();
	file.Close();
	MessageBox(_T("曲线已存入:")+FilePath);
}

void CCurveWnd::OnIdbCurveOk()
{
	//直接关闭
	::SendMessage(GetParent()->GetSafeHwnd(),WM_CLOSE,0,0);
}


void CCurveWnd::OnIdbCurveCancel()
{
	//恢复图像并退出

	CDocument* pDoc = GetDocument();
	if (m_pImage != NULL)
	{
		memcpy(m_pBits, m_dataSrc, m_nlenData);
		PIGetActiveView()->Invalidate(FALSE);
	}
//	::
	GetParent()->SendMessage(WM_CLOSE,0,0);
}


void CCurveWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_pImage == NULL) return;
	//按下鼠标左键，判断用户行为
	if (CursorIsInPaintRect(point))
	{
		RePlacePeg(point);//先看看是否修改peg
		m_bUserWillDrag = UserWillDrag(point);
	}
	CWnd::OnLButtonDown(nFlags, point);
}

void CCurveWnd::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_pImage == NULL) return;
	if (CursorIsInPaintRect(point) && m_bUserWillDrag)
	{//鼠标拖放结束，且终点不在pegs里面
		if (PegIsExist(point) == FALSE)
		{
			AddNewPeg(point);
		}
		ShowGrayTransform();
		ImshowImmediately();
		//更新m_bUserWillDrag
		m_bUserWillDrag = FALSE;
	}

	ReleaseCapture();
	CWnd::OnLButtonUp(nFlags, point);
}

void CCurveWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (CursorIsInPaintRect(point))
	{
		//显示当前灰度变换：输入/输出
		CString xPos, yPos;
		CPoint AfterTransform = TransPositionToRectangle(point);
		xPos.Format(_T("%d"), int(AfterTransform.x/m_fWidthRatio));
		yPos.Format(_T("%d"), int(AfterTransform.y/m_fHeightRatio));
		SetDlgItemText(IDE_CURVEWND_XPOSITION, xPos);
		SetDlgItemText(IDE_CURVEWND_YPOSITION, yPos);
		if (m_pImage == NULL) return;
		if (m_bUserWillDrag)
		{
			//AddNewPeg仅仅是为了拖动鼠标时能看到效果
			POSITION pos = AddNewPeg(point);
			ShowGrayTransform();
			UpdateWindow();//必须加上这句,才能看到当前拖动的peg
			//必须先释放指针指向的内存，否则将内存泄露
			if (pos != NULL)
			{
				delete m_pPegsList[m_nChannelSelected].GetAt(pos).m_pt;
				m_pPegsList[m_nChannelSelected].RemoveAt(pos);
			}
		}
	}
	else
	{
		SetDlgItemText(IDE_CURVEWND_XPOSITION, _T(""));
		SetDlgItemText(IDE_CURVEWND_YPOSITION, _T(""));
	}
	CWnd::OnMouseMove(nFlags, point);
}

//
// *** 三次样条函数 ***
//

VECTOR CCurveWnd::Spline(VECTOR& Node, VECTOR& Value, VECTOR& x0, double c1, double c2, int CASE)
{
	//CASE：边界条件的情形
	//c1：边界条件1
	//c2：边界条件2
	VECTOR u, Lamda, d, Mj, h, f1, s;
	double temp, f2;
	int i, j, n = Node.size(), sta = 0;
	for(j = 1; j<n; j++)
	{
		h.push_back(Node[j]-Node[j-1]);
		f1.push_back((Value[j]-Value[j-1])/h[j-1]);
	}
	u.push_back(0);
	Lamda.push_back(0);
	d.push_back(0);
	for(j = 1; j<n-1; j++)
	{
		u.push_back(h[j-1]/(h[j-1]+h[j]));
		Lamda.push_back(h[j]/(h[j-1]+h[j]));
		f2 = (f1[j]-f1[j-1])/(Node[j+1]-Node[j-1]);
		d.push_back(6*f2);
	}

	//边界条件
	switch(CASE)
	{
	case 1:
		u.push_back(1);
		Lamda[0] = 1;
		d[0] = 6*(f1[0]-c1)/h[0];
		d.push_back(6*(c2-f1[n-2])/h[n-2]);
		break;
	case 2:
		u.push_back(0);
		Lamda[0] = 0;
		d[0] = 2*c1;
		d.push_back(2*c2);
		break;
	default:
		return VECTOR(0);
	}

	VECTOR b;
	for(i = 0; i<n; i++)
		b.push_back(2.0);
	u[0] = 0;
	Lamda.push_back(0);
	//追赶法求解
	Chase(u, b, Lamda, d, Mj, n);
	for (i = 0; i<int(x0.size()); i++)
	{
		if(x0[i]<Node[0])
		{
			j = 0;
			temp = Mj[j]*pow(Node[j+1]-x0[i],3)/(6*h[j])+Mj[j+1]*pow(x0[i]-Node[j],3)/(6*h[j])+(Value[j]-Mj[j]*pow(h[j],2)/6)*(Node[j+1]-x0[i])/h[j]+(Value[j+1]-Mj[j+1]*pow(h[j],2)/6)*(x0[i]-Node[j])/h[j];
			s.push_back(temp);
			continue;
		}
		if(x0[i]>Node[n-1])
		{
			j = n-2;
			temp = Mj[j]*pow(Node[j+1]-x0[i],3)/(6*h[j])+Mj[j+1]*pow(x0[i]-Node[j],3)/(6*h[j])+(Value[j]-Mj[j]*pow(h[j],2)/6)*(Node[j+1]-x0[i])/h[j]+(Value[j+1]-Mj[j+1]*pow(h[j],2)/6)*(x0[i]-Node[j])/h[j];
			s.push_back(temp);
			continue;
		}
		for(j = 0; j<n-1; j++)
		{
			if(Node[j]<=x0[i] && x0[i]<=Node[j+1])
			{
				temp= Mj[j]*pow(Node[j+1]-x0[i],3)/(6*h[j])+Mj[j+1]*pow(x0[i]-Node[j],3)/(6*h[j])+(Value[j]-Mj[j]*pow(h[j],2)/6)*(Node[j+1]-x0[i])/h[j]+(Value[j+1]-Mj[j+1]*pow(h[j],2)/6)*(x0[i]-Node[j])/h[j];
				s.push_back(temp);
				break;
			}
		}
	}

	u.~vector();
	Lamda.~vector();
	d.~vector();
	Mj.~vector();
	h.~vector();
	f1.~vector();
	b.~vector();
	return s;
}

//追赶法
void CCurveWnd::Chase(VECTOR& a, VECTOR& b, VECTOR& c, VECTOR& f, VECTOR& x, int n)
{
	//a,b,c：三对角线元素
	//x：方程的解
	//n：矩阵的行
	int i;
	VECTOR belta;
	double tmp;
	//追的过程
	belta.push_back(c[0]/b[0]);
	x.push_back(f[0]/b[0]);
	for(i = 1; i<n-1; i++)
	{
		tmp = b[i]-a[i]*belta[i-1];
		belta.push_back(c[i]/tmp);
		x.push_back((f[i]-a[i]*x[i-1])/tmp);
	}
	tmp = b[i]-a[i]*belta[i-1];
	x.push_back((f[i]-a[i]*x[i-1])/tmp);
	//赶的过程
	for(i = n-2; i>=0; i--)
	{
		x[i] = x[i]-belta[i]*x[i+1];
	}
	belta.~vector();
}

//本来是设置通道颜色,为了使颜色条也能使用这个函数,WhereUse此参数仅仅为了节省代码量
COLORREF CCurveWnd::ColorSetting(int Channel, int Strength, int WhereUse)
{
	//Channel:通道
	//Strength:颜色强度
	COLORREF LineColor;
	switch(Channel)
	{
	case 0:
		if (WhereUse)
		{
			Strength = 255 - Strength;
		}
		LineColor = RGB(255-Strength, 255-Strength, 255-Strength);
		break;
	case 1:
		LineColor = RGB(Strength, 0, 0);
		break;
	case 2:
		LineColor = RGB(0, Strength, 0);
		break;
	case 3:
		LineColor = RGB(0, 0, Strength);
		break;
	default:
		break;
	}
	return LineColor;
}

void CCurveWnd::OnRButtonUp(UINT nFlags, CPoint point)
{
	if (CursorIsInPaintRect(point))
	{
		if (PegIsExist(point) == TRUE)
		{
			delete m_pPegsList[m_nChannelSelected].GetAt(m_posCurrentPeg).m_pt;
			m_pPegsList[m_nChannelSelected].RemoveAt(m_posCurrentPeg);

			ShowGrayTransform();
			GrayTransform();
			ImshowImmediately();
		}
	}
	CWnd::OnRButtonUp(nFlags, point);
}

void CCurveWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	GetClientRect(m_pWndSize);

	//绘图区Rect
	m_pOnPaintRect->right = m_pWndSize->right-70;
	m_pOnPaintRect->bottom = m_pWndSize->bottom-80;

	//宽高相对于255缩放比
	m_nRectWidth = m_pOnPaintRect->Width();
	m_nRectHeight = m_pOnPaintRect->Height();
	m_fWidthRatio = m_nRectWidth/255.0;
	m_fHeightRatio = m_nRectHeight/255.0;

	//修改peg的坐标
	ReshapePegs();
	
	//修改灰度变换函数
	ReshapeTransform();

	int nButtonPosX1 = m_pWndSize->right-50;
	int nButtonPosX2 = nButtonPosX1+BUTTON_WIDTH;
	double Space = m_pWndSize->Height()/9.0;

	//Group Box
	CWnd* hWnd = GetDlgItem(IDB_GROUP_BOX);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(20, 20, m_pWndSize->right-80, m_pWndSize->bottom-80);
	}
	//选择曲线
	ReShapeButtons(IDB_CHOOSE_CURVE, nButtonPosX1, nButtonPosX2, Space, 1);
	//选择直线
	ReShapeButtons(IDB_CHOOSE_LINE, nButtonPosX1, nButtonPosX2, Space, 2);
	//重置
	ReShapeButtons(IDB_CURVEWND_RESET, nButtonPosX1, nButtonPosX2, Space, 3);
	//反转
	ReShapeButtons(IDB_CURVEWND_INVERSE, nButtonPosX1, nButtonPosX2, Space, 4);
	//导入曲线
	ReShapeButtons(IDB_IMPORT_CURVE, nButtonPosX1, nButtonPosX2, Space, 5);
	//导出曲线
	ReShapeButtons(IDB_EXPORT_CURVE, nButtonPosX1, nButtonPosX2, Space, 6);
	//X坐标
	hWnd = GetDlgItem(IDE_CURVEWND_XPOSITION);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(nButtonPosX1+15, int(7*Space), BUTTON_WIDTH-15, BUTTON_HEIGHT);
	}
	//Y坐标
	hWnd = GetDlgItem(IDE_CURVEWND_YPOSITION);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(nButtonPosX1+15, int(8*Space), BUTTON_WIDTH-15, BUTTON_HEIGHT);
	}
	//X标签
	hWnd = GetDlgItem(IDS_CURVEWND_XLABEL);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(nButtonPosX1, int(7*Space), BUTTON_WIDTH-25, BUTTON_HEIGHT);
	}
	//Y标签
	hWnd = GetDlgItem(IDS_CURVEWND_YLABEL);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(nButtonPosX1, int(8*Space), BUTTON_WIDTH-25, BUTTON_HEIGHT);
	}

	//OK
	hWnd = GetDlgItem(IDB_CURVE_OK);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(40, m_pWndSize->bottom-40, BUTTON_WIDTH, BUTTON_HEIGHT);
	}
	//CANCEL
	hWnd = GetDlgItem(IDB_CURVE_CANCEL);
	if (hWnd != NULL)
	{
		hWnd->MoveWindow(m_pWndSize->right-70-BUTTON_WIDTH, m_pWndSize->bottom-40, BUTTON_WIDTH, BUTTON_HEIGHT);
	}
	if (m_pImage == NULL) return;
	//仅当m_pImage非空时候执行下一句
	ShowGrayTransform();
}

//窗口变化时，调整按钮位置
void CCurveWnd::ReShapeButtons(int nItemID, int nButtonPosX1, int nButtonPosX2, double Space, int Number)
{
	CWnd *hWnd = GetDlgItem(nItemID);
	if (hWnd != NULL)
	{//MoveWindow:int x, int y, int nWidth, int nHeight
		hWnd->MoveWindow(nButtonPosX1, int(Number*Space-10), BUTTON_WIDTH, BUTTON_HEIGHT);
	}
}

//窗口变化时，Pegs坐标同时变化
void CCurveWnd::ReshapePegs()
{
	POSITION pos = m_pPegsList[m_nChannelSelected].GetHeadPosition();
	while (pos != NULL)
	{
		peg& Temp = m_pPegsList[m_nChannelSelected].GetNext(pos);
		//相对于Rect左上角位置
		Temp.m_pt->x = LONG(m_pOnPaintRect->left+Temp.m_dRatioX*m_nRectWidth);
		Temp.m_pt->y = LONG(m_pOnPaintRect->top+Temp.m_dRatioY*m_nRectHeight);
	}
}

//窗口变化时，存放灰度变换的vector也变化
void CCurveWnd::ReshapeTransform()
{
	if (m_pImage == NULL) return;
	for (int i = 0; i<4; i++)
	{
		m_V4Transform[m_nChannelSelected].clear();
		for (int j = 0; j<m_nRectWidth+1; j++)
		{
			m_V4Transform[m_nChannelSelected].push_back(0);
		}
	}
}

BOOL CCurveWnd::OnEraseBkgnd(CDC* pDC)
{
	//使其不要重绘背景
	return TRUE;
}