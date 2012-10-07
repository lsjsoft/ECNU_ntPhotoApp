#pragma once

#include "Resource.h"
#include "afxwin.h"
#include "ntCanvasCtl.h"
#include "GeWinAnchor.h"

class ntDlgSelTrans: public CDialog
{
	DECLARE_DYNAMIC(ntDlgSelTrans);
	enum { IDD = IDD_TRANS_SEL };

	DECLARE_MESSAGE_MAP();
public:
	ntDlgSelTrans(CWnd* pParent = NULL);
public:
	~ntDlgSelTrans(void);
	void DoDataExchange(CDataExchange* pDX);
public:
	int m_iValue;
};
