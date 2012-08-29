// ChildFrm.h : interface of the ntChildFrame class
//


#pragma once


class ntChildFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(ntChildFrame)
public:
	ntChildFrame();

// Attributes
public:

// Operations
public:

// Overrides
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnMDIActivate(BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd);
	afx_msg void OnPlugMenuClick(UINT uId);

// Implementation
public:
	virtual ~ntChildFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	std::vector<std::string> m_extendPlugList;

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};
