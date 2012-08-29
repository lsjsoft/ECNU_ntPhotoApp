// ntPhotoAppView.h : interface of the ntPhotoAppView class
//


#pragma once
class ntPhotoAppDoc;

#include "ntTexture.h"

class ntScreen;

class ntPhotoAppView : public CView
{
protected: // create from serialization only
	ntPhotoAppView();
	DECLARE_DYNCREATE(ntPhotoAppView)

	// Attributes
public:
	ntPhotoAppDoc* GetDocument() const;
	virtual void OnInitialUpdate();
	// Operations
public:
	void DoRefresh()
	{
		renderView();
	}
	// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	void renderView();

	// Implementation
public:
	virtual ~ntPhotoAppView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);

protected:
	ntScreen* m_pScreen;
	ntTexture32Ptr m_background;

	// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnEditUndo();
	afx_msg void OnUpdateEditUndo(CCmdUI *pCmdUI);
	afx_msg void OnEditRedo();
	afx_msg void OnUpdateEditRedo(CCmdUI *pCmdUI);
	afx_msg void OnUpdatePlugRange(CCmdUI *pCmdU);
};

#ifndef _DEBUG  // debug version in ntPhotoAppView.cpp
inline ntPhotoAppDoc* ntPhotoAppView::GetDocument() const
{ return reinterpret_cast<ntPhotoAppDoc*>(m_pDocument); }
#endif

