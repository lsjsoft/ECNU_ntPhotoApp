#pragma once
#include "afxcmn.h"


// plugContrastMain dialog

class plugContrastMain : public CDialog
{
	DECLARE_DYNAMIC(plugContrastMain)

public:
	plugContrastMain(ntPlugCallParam* pParam, CWnd* pParent = NULL);   // standard constructor
	virtual ~plugContrastMain();

// Dialog Data
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	void Refresh();

	DECLARE_MESSAGE_MAP()

	ntPlugCallParam* m_pParam;
public:
	BOOL m_bPreview;
public:
	int m_iAdjustValue;
	int m_iGrayAvg;
public:
	CSliderCtrl m_AdjustCtrl;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
public:
	afx_msg void OnBnClickedPreview();
public:
	afx_msg void OnNMReleasedcaptureAdjustvalue(NMHDR *pNMHDR, LRESULT *pResult);
};
