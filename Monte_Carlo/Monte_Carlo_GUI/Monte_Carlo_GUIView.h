
// Monte_Carlo_GUIView.h : interface of the CMonte_Carlo_GUIView class
//

#pragma once


class CMonte_Carlo_GUIView : public CView
{
protected: // create from serialization only
	CMonte_Carlo_GUIView();
	DECLARE_DYNCREATE(CMonte_Carlo_GUIView)

// Attributes
public:
	CMonte_Carlo_GUIDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CMonte_Carlo_GUIView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in Monte_Carlo_GUIView.cpp
inline CMonte_Carlo_GUIDoc* CMonte_Carlo_GUIView::GetDocument() const
   { return reinterpret_cast<CMonte_Carlo_GUIDoc*>(m_pDocument); }
#endif

