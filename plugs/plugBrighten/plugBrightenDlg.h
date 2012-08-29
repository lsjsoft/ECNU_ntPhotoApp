#pragma once
#include "afxcmn.h"


// plugBrightenDlg dialog

class plugBrightenDlg : public CDialog
{
	DECLARE_DYNAMIC(plugBrightenDlg)

public:
	plugBrightenDlg(ntPlugCallParam* pParam, CWnd* pParent = NULL);   // standard constructor
	virtual ~plugBrightenDlg();

// Dialog Data
	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPreview();
public:
	BOOL m_bPreview;
public:
	int m_iAdjustValue;
public:
	CSliderCtrl m_AjdustCtrl;
public:
	afx_msg void OnNMReleasedcaptureAdjustvalue(NMHDR *pNMHDR, LRESULT *pResult);

public:
	void Refresh();

protected:
	ntPlugCallParam* m_pParam;
public:
	afx_msg void OnBnClickedOk();
public:
	virtual BOOL OnInitDialog();
};
