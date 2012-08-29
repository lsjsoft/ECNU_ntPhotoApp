// plugBrightenDlg.cpp : implementation file
//

#include "stdafx.h"
#include "plugBrighten.h"
#include "plugBrightenDlg.h"

#define LimitColor(c)		{ c < 0 ? c = 0 : 0; c > 255 ? c = 255 : 0; }

static void DoProcess(ntPlugPixData* pDest, ntPlugPixData* pSrc, int Lv)
{
	ntPlugPix* pDestPix= pDest->m_pPixelData;
	ntPlugPix* pSrcPix= pSrc->m_pPixelData;

	unsigned char table[256];
	for(int i=0; i<256; ++i)
	{
		int c = i+ Lv;
		LimitColor(c);
		table[i] = c;
	}


	int uiSize= pSrc->m_uiWidth * pSrc->m_uiHeight;
	ntPlugPix* src= pSrc->m_pPixelData;
	ntPlugPix* dest= pDest->m_pPixelData;
	int uiCounter= 0;

	while(uiCounter++<uiSize)
	{
		dest->r = table[src->r];
		dest->g = table[src->g];
		dest->b = table[src->b];
		++dest;
		++src;
	}
}

// plugBrightenDlg dialog

IMPLEMENT_DYNAMIC(plugBrightenDlg, CDialog)

plugBrightenDlg::plugBrightenDlg(ntPlugCallParam* pParam, CWnd* pParent /*=NULL*/)
	: CDialog(plugBrightenDlg::IDD, pParent)
	, m_bPreview(TRUE)
	, m_iAdjustValue(256)
	, m_pParam(pParam)
{
	
}

plugBrightenDlg::~plugBrightenDlg()
{
}

void plugBrightenDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PREVIEW, m_bPreview);
	DDX_Slider(pDX, IDC_ADJUSTVALUE, m_iAdjustValue);
	DDX_Control(pDX, IDC_ADJUSTVALUE, m_AjdustCtrl);
}


BEGIN_MESSAGE_MAP(plugBrightenDlg, CDialog)
	ON_BN_CLICKED(IDC_PREVIEW, &plugBrightenDlg::OnBnClickedPreview)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ADJUSTVALUE, &plugBrightenDlg::OnNMReleasedcaptureAdjustvalue)
	ON_BN_CLICKED(IDOK, &plugBrightenDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// plugBrightenDlg message handlers

void plugBrightenDlg::OnBnClickedPreview()
{
	Refresh();
	// TODO: Add your control notification handler code here
}

void plugBrightenDlg::OnNMReleasedcaptureAdjustvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: Add your control notification handler code here
	*pResult = 0;

	Refresh();
}

void plugBrightenDlg::Refresh()
{
	UpdateData();

	if (!m_bPreview)
	{
		m_pParam->restoreSource();
	}
	else
	{
		DoProcess( m_pParam->pSource, m_pParam->pSourceBackup, m_iAdjustValue-256);
	}

	m_pParam->pPreviewFunc();
}


void plugBrightenDlg::OnBnClickedOk()
{
	UpdateData();

	unsigned int w = m_pParam->pSource->m_uiWidth;
	unsigned int h = m_pParam->pSource->m_uiHeight;
	m_pParam->pAllFunc(m_pParam->pOutput, w, h);

	if (m_bPreview)
	{
		(*m_pParam->pOutput)->copyFrom( m_pParam->pSource );
	}
	else
	{
		DoProcess( (*m_pParam->pOutput), m_pParam->pSourceBackup, m_iAdjustValue-256 );
	}

	OnOK();
}

BOOL plugBrightenDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_AjdustCtrl.SetRange(0, 512);
	m_AjdustCtrl.SetPos(256);

	UpdateData(FALSE);

	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
