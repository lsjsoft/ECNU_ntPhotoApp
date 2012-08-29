// MainFrm.h : interface of the ntMainFrame class
//


#pragma once

class ntHistogramView;

class ntMainFrame : public CMDIFrameWnd
{
	DECLARE_DYNAMIC(ntMainFrame)
public:
	ntMainFrame();

// Attributes
public:

// Operations
public:
	virtual void GetMessageString(UINT nID, CString& rMessage) const;

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~ntMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;

	afx_msg void OnFileSave();

// Generated message map functions
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnViewHistogram();
	afx_msg LRESULT OnUpdateActiveHistogram(WPARAM wParam, LPARAM lParam);

protected:
	ntHistogramView* m_pHistogramView;
};


