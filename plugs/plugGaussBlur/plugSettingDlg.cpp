// plugSettingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "plugGaussBlur.h"
#include "plugSettingDlg.h"

class ntPlugPixF
{
public:
	ntPlugPixF(): a(0), r(0), g(0), b(0), c(0) {}

	const ntPlugPixF& operator += ( const ntPlugPix& p)
	{
		a+= p.a;
		r+= p.r;
		g+= p.g;
		b+= p.b;
		c++;
		return *this;
	}

	void add(const ntPlugPix& p, float f)
	{
		float f1= f / 255.0f;

		a +=  f1 * p.a;
		r +=  f1 * p.r;
		g +=  f1 * p.g;
		b +=  f1 * p.b;
	}

	void setPix3(ntPlugPix& p)
	{
		p.a = a * 255.0f;
		p.r = r * 255.0f;
		p.g = g * 255.0f;
		p.b = b * 255.0f;
	}

	void setPix(ntPlugPix& p)
	{
		p.a = a / c;
		p.r = r / c;
		p.g = g / c;
		p.b = b / c;
	}

	float a, r, g, b, c;
};

/*
sharpeness,			sharpeness*2,			sharpeness,
sharpeness*2,		1 - 12 * sharpeness,	sharpeness*2,
sharpeness,			sharpeness*2,			sharpeness
*/

void DoProcess3(ntPlugPixData* pDest, ntPlugPixData* pSrc, float sharpeness=-0.075f)
{
	float m[3][3];
	m[0][0] = sharpeness;
	m[0][1] = sharpeness*2.0f;
	m[0][2] = sharpeness;

	m[1][0] = sharpeness*2.0f;
	m[1][1] = 1.0f - sharpeness*12.0f;
	m[1][2] = sharpeness*2.0f;

	m[2][0] = sharpeness;
	m[2][1] = sharpeness*2.0f;
	m[2][2] = sharpeness;

	ntPlugPix* pDestPix= pDest->m_pPixelData;
	ntPlugPix* pSrcPix= pSrc->m_pPixelData;

	for(int y = 0; y< pDest->m_uiHeight; ++y)
	{
		for(int x = 0; x< pDest->m_uiWidth; ++x)
		{
			ntPlugPixF f;

			for(int y0 = 0-1; y0 <= 1; ++y0 )
			{
				for(int x0 = 0-1; x0 <= 1; ++x0)
				{
					int ax = x + x0;
					int ay = y + y0;

					if ( ax>=0 && ay>=0 && ax<pDest->m_uiWidth 
						&& ay<pDest->m_uiHeight )
					{
						f.add( pSrcPix[pDest->m_uiWidth*ay+ax], m[y0+1][x0+1] );
					}
				}
			}

			f.setPix3( pDestPix[pDest->m_uiWidth*y+x] );
		}
	}
}


void DoProcess(ntPlugPixData* pDest, ntPlugPixData* pSrc, int Lv= 1)
{
	ntPlugPix* pDestPix= pDest->m_pPixelData;
	ntPlugPix* pSrcPix= pSrc->m_pPixelData;

	for(int y = 0; y< pDest->m_uiHeight; ++y)
	{
		for(int x = 0; x< pDest->m_uiWidth; ++x)
		{
			ntPlugPixF f;

			for(int y0 = 0-Lv; y0 <= Lv; ++y0 )
			{
				for(int x0 = 0-Lv; x0 <= Lv; ++x0)
				{
					int ax = x + x0;
					int ay = y + y0;

					if ( ax>=0 && ay>=0 && ax<pDest->m_uiWidth 
						&& ay<pDest->m_uiHeight )
					{
						f += pSrcPix[pDest->m_uiWidth*ay+ax];
					}
				}
			}

			f.setPix( pDestPix[pDest->m_uiWidth*y+x] );
		}
	}
}

// plugSettingDlg dialog

IMPLEMENT_DYNAMIC(plugSettingDlg, CDialog)

plugSettingDlg::plugSettingDlg(ntPlugCallParam* pParam, CWnd* pParent /*=NULL*/)
	: CDialog(plugSettingDlg::IDD, pParent)
	, m_pParam(pParam)
	, m_bPreview(TRUE)
	, m_BlurLv(1)
	, m_bSimple(FALSE)
	, m_fSharpness(-0.075f)
{
	unsigned int w = pParam->pSource->m_uiWidth;
	unsigned int h = pParam->pSource->m_uiHeight;
	pParam->pAllFunc(pParam->pOutput, w, h);
}

plugSettingDlg::~plugSettingDlg()
{
}

void plugSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Check(pDX, IDC_PREVIEW, m_bPreview);
	DDX_Text(pDX, IDC_BLUR_LV, m_BlurLv);
	DDV_MinMaxUInt(pDX, m_BlurLv, 1, 6);
	DDX_Check(pDX, IDC_USESIMPLE, m_bSimple);
	DDX_Text(pDX, IDC_SHARPNESS, m_fSharpness);
	DDV_MinMaxFloat(pDX, m_fSharpness, -10.0f, 10.0f);
}


BEGIN_MESSAGE_MAP(plugSettingDlg, CDialog)
	ON_BN_CLICKED(IDC_PREVIEW, &plugSettingDlg::OnBnClickedPreview)
	ON_BN_CLICKED(IDOK, &plugSettingDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_BLUR_LV, &plugSettingDlg::OnEnChangeBlurLv)
	ON_BN_CLICKED(IDC_USESIMPLE, &plugSettingDlg::OnBnClickedUsesimple)
	ON_EN_CHANGE(IDC_SHARPNESS, &plugSettingDlg::OnEnChangeSharpness)
END_MESSAGE_MAP()


// plugSettingDlg message handlers

void plugSettingDlg::OnBnClickedPreview()
{
	// TODO: Add your control notification handler code here
	UpdateData();

	Refresh();
}

void plugSettingDlg::OnBnClickedOk()
{
	if (m_bPreview)
	{
		(*m_pParam->pOutput)->copyFrom( m_pParam->pSource );
	}
	else
	{
		DoProcess( (*m_pParam->pOutput), m_pParam->pSourceBackup, m_BlurLv );
	}

	// TODO: Add your control notification handler code here
	OnOK();
}

void plugSettingDlg::OnEnChangeBlurLv()
{
	// TODO:  If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CDialog::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.

	// TODO:  Add your control notification handler code here
	UpdateData();

	Refresh();
}

void plugSettingDlg::Refresh()
{
	if (!m_bPreview)
	{
		m_pParam->restoreSource();
	}
	else
	{
		if (!m_bSimple)
		{
			DoProcess( m_pParam->pSource, m_pParam->pSourceBackup, m_BlurLv );
		}
		else
		{
			DoProcess3( m_pParam->pSource, m_pParam->pSourceBackup, m_fSharpness );
		}
	}

	m_pParam->pPreviewFunc();
}
void plugSettingDlg::OnBnClickedUsesimple()
{
	UpdateData();

	Refresh();
}

void plugSettingDlg::OnEnChangeSharpness()
{
	UpdateData();

	Refresh();
}

BOOL plugSettingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	OnEnChangeSharpness();
	// TODO:  Add extra initialization here

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}
