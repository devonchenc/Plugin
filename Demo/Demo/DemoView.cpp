
// DemoView.cpp : implementation of the CDemoView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Demo.h"
#endif

#include "DemoDoc.h"
#include "DemoView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CDemoView

IMPLEMENT_DYNCREATE(CDemoView, CView)

BEGIN_MESSAGE_MAP(CDemoView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CDemoView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CDemoView construction/destruction

CDemoView::CDemoView()
{
	// TODO: add construction code here

}

CDemoView::~CDemoView()
{
}

BOOL CDemoView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CDemoView drawing

void CDemoView::OnDraw(CDC* /*pDC*/)
{
	CDemoDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CDemoView printing


void CDemoView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CDemoView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CDemoView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CDemoView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CDemoView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CDemoView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CDemoView diagnostics

#ifdef _DEBUG
void CDemoView::AssertValid() const
{
	CView::AssertValid();
}

void CDemoView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CDemoDoc* CDemoView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDemoDoc)));
	return (CDemoDoc*)m_pDocument;
}
#endif //_DEBUG


// CDemoView message handlers
