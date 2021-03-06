// ntPhotoAppView.h : interface of the ntPhotoAppView class
//


#pragma once
class ntPhotoAppDoc;

#include "ntTexture.h"

class ntScreen;
class ntOpMgr;

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

	ntOpMgr* getOpMgr()
	{
		return m_pOpMgr;
	}
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
	ntOpMgr* m_pOpMgr;
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
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
public:
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

#ifndef _DEBUG  // debug version in ntPhotoAppView.cpp
inline ntPhotoAppDoc* ntPhotoAppView::GetDocument() const
{ return reinterpret_cast<ntPhotoAppDoc*>(m_pDocument); }
#endif

