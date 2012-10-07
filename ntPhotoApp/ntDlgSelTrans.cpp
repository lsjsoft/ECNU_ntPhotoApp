#include "StdAfx.h"
#include "ntDlgSelTrans.h"


BEGIN_MESSAGE_MAP(ntDlgSelTrans, CDialog)
	//	ON_WM_DRAWITEM()
END_MESSAGE_MAP()

IMPLEMENT_DYNAMIC(ntDlgSelTrans, CDialog)

void ntDlgSelTrans::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TRANS_VALUE, m_iValue);
	DDV_MinMaxInt(pDX, m_iValue, 0, 100);
}

ntDlgSelTrans::ntDlgSelTrans(CWnd* pParent)
: CDialog(ntDlgSelTrans::IDD, pParent)
, m_iValue(100)
{
}

ntDlgSelTrans::~ntDlgSelTrans(void)
{
}
