
// Monte_Carlo_GUIView.cpp : implementation of the CMonte_Carlo_GUIView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "Monte_Carlo_GUI.h"
#endif

#include "Monte_Carlo_GUIDoc.h"
#include "Monte_Carlo_GUIView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMonte_Carlo_GUIView

IMPLEMENT_DYNCREATE(CMonte_Carlo_GUIView, CView)

BEGIN_MESSAGE_MAP(CMonte_Carlo_GUIView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMonte_Carlo_GUIView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()

// CMonte_Carlo_GUIView construction/destruction

CMonte_Carlo_GUIView::CMonte_Carlo_GUIView()
{
	// TODO: add construction code here

}

CMonte_Carlo_GUIView::~CMonte_Carlo_GUIView()
{
}

BOOL CMonte_Carlo_GUIView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CMonte_Carlo_GUIView drawing

void CMonte_Carlo_GUIView::OnDraw(CDC* /*pDC*/)
{
	CMonte_Carlo_GUIDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: add draw code for native data here
}


// CMonte_Carlo_GUIView printing


void CMonte_Carlo_GUIView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMonte_Carlo_GUIView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CMonte_Carlo_GUIView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CMonte_Carlo_GUIView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CMonte_Carlo_GUIView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CMonte_Carlo_GUIView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMonte_Carlo_GUIView diagnostics

#ifdef _DEBUG
void CMonte_Carlo_GUIView::AssertValid() const
{
	CView::AssertValid();
}

void CMonte_Carlo_GUIView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMonte_Carlo_GUIDoc* CMonte_Carlo_GUIView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMonte_Carlo_GUIDoc)));
	return (CMonte_Carlo_GUIDoc*)m_pDocument;
}
#endif //_DEBUG


// CMonte_Carlo_GUIView message handlers
