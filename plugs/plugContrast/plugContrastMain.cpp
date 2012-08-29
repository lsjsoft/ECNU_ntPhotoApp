// plugContrastMain.cpp : implementation file
//

#include "stdafx.h"
#include "plugContrast.h"
#include "plugContrastMain.h"

#define MakeGray(p)			( ( p->r * 76 + p->g * 150 + p->b * 29 ) >> 8 )
#define LimitColor(c)		{ c < 0 ? c = 0 : 0; c > 255 ? c = 255 : 0; }

unsigned int GetAvgGrayValue(ntPlugPixData* pSrc)
{
	ntPlugPix* p= pSrc->m_pPixelData;
	int uiSize= pSrc->m_uiWidth * pSrc->m_uiHeight;
	int uiCounter= 0;
	int uiTotal = 0;
	while( uiCounter++ < uiSize)
	{
		uiTotal += MakeGray(p);
		++p;
	}
	uiTotal/= uiCounter;
	return uiTotal;
}

void DoProcess(ntPlugPixData* pDest, ntPlugPixData* pSrc, int Lv, int iAvg)
{
	ntPlugPix* pDestPix= pDest->m_pPixelData;
	ntPlugPix* pSrcPix= pSrc->m_pPixelData;

	unsigned char table[256];
	for(int i=0; i<256; ++i)
	{
		int c = iAvg + (i-iAvg) * Lv / 100;
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

IMPLEMENT_DYNAMIC(plugContrastMain, CDialog)

plugContrastMain::plugContrastMain(ntPlugCallParam* pParam, CWnd* pParent /*=NULL*/)
	: CDialog(plugContrastMain::IDD, pParent)
	, m_pParam(pParam)
	, m_bPreview(TRUE)
	, m_iAdjustValue(0)
{
	m_iGrayAvg = GetAvgGrayValue(m_pParam->pSource);
}

plugContrastMain::~plugContrastMain()
{
}

void plugContrastMain::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PREVIEW, m_bPreview);
	DDX_Slider(pDX, IDC_ADJUSTVALUE, m_iAdjustValue);
	DDX_Control(pDX, IDC_ADJUSTVALUE, m_AdjustCtrl);
}


BEGIN_MESSAGE_MAP(plugContrastMain, CDialog)
	ON_BN_CLICKED(IDOK, &plugContrastMain::OnBnClickedOk)
	ON_BN_CLICKED(IDC_PREVIEW, &plugContrastMain::OnBnClickedPreview)

	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_ADJUSTVALUE, 
	&plugContrastMain::OnNMReleasedcaptureAdjustvalue)

END_MESSAGE_MAP()

void plugContrastMain::Refresh()
{
	if (!m_bPreview)
	{
		m_pParam->restoreSource();
	}
	else
	{
		DoProcess( m_pParam->pSource, m_pParam->pSourceBackup, m_iAdjustValue, m_iGrayAvg);
	}

	m_pParam->pPreviewFunc();
}

void plugContrastMain::OnBnClickedOk()
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
		DoProcess( (*m_pParam->pOutput), m_pParam->pSourceBackup, m_iAdjustValue, m_iGrayAvg );
	}

	OnOK();
}

BOOL plugContrastMain::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_AdjustCtrl.SetRange(0, 200);
	m_AdjustCtrl.SetPos( 100 );

	return TRUE;
}

void plugContrastMain::OnBnClickedPreview()
{
	UpdateData();

	Refresh();
}


void plugContrastMain::OnNMReleasedcaptureAdjustvalue(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 0;

	UpdateData();

	Refresh();
}
