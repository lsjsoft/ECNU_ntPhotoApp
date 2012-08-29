#pragma once


// plugSettingDlg dialog

class plugSettingDlg : public CDialog
{
	DECLARE_DYNAMIC(plugSettingDlg)

public:
	plugSettingDlg(ntPlugCallParam*, CWnd* pParent = NULL);   // standard constructor
	virtual ~plugSettingDlg();

// Dialog Data
	enum { IDD = IDD_MAINDLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()

protected:
	ntPlugCallParam* m_pParam;
public:
	afx_msg void OnBnClickedPreview();
public:
	BOOL m_bPreview;
public:
	afx_msg void OnBnClickedOk();
public:
	UINT m_BlurLv;
	float m_fSharpness;
public:
	afx_msg void OnEnChangeBlurLv();

	void Refresh();
public:
	afx_msg void OnBnClickedUsesimple();
public:
	afx_msg void OnEnChangeSharpness();
public:
	BOOL m_bSimple;
public:
	virtual BOOL OnInitDialog();
};
