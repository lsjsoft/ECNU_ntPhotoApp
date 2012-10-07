#pragma once
#include "afxwin.h"
#include "plugOS.h"

class GeFontFreeTypeImpl;

// plugMainDlg dialog

class CMyStatic: public CStatic
{
public:
	void setColor(DWORD dwColor);

protected:
	void OnPaint();

	DECLARE_MESSAGE_MAP()


	DWORD m_DrawColor;
};


class plugMainDlg : public CDialog
{
	DECLARE_DYNAMIC(plugMainDlg)

public:
	plugMainDlg(ntPlugCallParam* pParam, CWnd* pParent = NULL);   // standard constructor
	virtual ~plugMainDlg();

	enum { IDD = IDD_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnSelchangeFontType();
	void ResetFont();
	afx_msg void OnEnChangeContent();
	void RefreshText();
	afx_msg void OnEnChangeFontSize();
	afx_msg void OnBnClickedPreview();
	afx_msg void OnBnClickedSelColor();

	void Process(ntPlugPixData* pData);

	void DrawFreeText( std::wstring &wstr, ntPlugPixData* pData, int x, int y, DWORD dwColor );
protected:
	UINT m_uFontSize;
	plugOsFontInfos m_fontInfoList;
	GeFontFreeTypeImpl* m_freetypeFont;
	ntPlugCallParam* m_pParam;
	CComboBox m_FontTypeCtl;
	CEdit m_FontSize;
	CString m_Content;
	BOOL m_bPreview;
	DWORD m_dwColor;
public:
	CMyStatic m_ColorView;
	int m_x;
	int m_y;
	afx_msg void OnBnClickedOk();
	BOOL m_shoke;
	afx_msg void OnBnClickedShoke();
	UINT m_iTransV;
	afx_msg void OnEnChangeTransv();
};
