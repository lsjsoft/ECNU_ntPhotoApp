// plugMainDlg.cpp : implementation file
//

#include "stdafx.h"
#include "plugText.h"
#include "plugMainDlg.h"
#include "plugOS.h"
#include "plugVectorFont.h"
#include "GeUnicodeHelper.h"


// plugMainDlg dialog

IMPLEMENT_DYNAMIC(plugMainDlg, CDialog)

plugMainDlg::plugMainDlg(ntPlugCallParam* pParam, CWnd* pParent /*=NULL*/)
	: CDialog(plugMainDlg::IDD, pParent)
	, m_Content(_T("EAST CHINA NORMAL UNIVERSITY\r\nCOMPUTER SCIENCE AND TECHNOLOGY"))
	, m_bPreview(TRUE)
	, m_uFontSize(30)
	, m_freetypeFont(0)
	, m_pParam(pParam)
	, m_dwColor(0x00000000)
	, m_x(10)
	, m_y(10)
	, m_shoke(FALSE)
	, m_iTransV(80)
{
}

plugMainDlg::~plugMainDlg()
{
	delete m_freetypeFont;
}

void plugMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FONT_TYPE, m_FontTypeCtl);
	DDX_Control(pDX, IDC_FONT_SIZE, m_FontSize);
	DDX_Text(pDX, IDC_CONTENT, m_Content);
	DDX_Check(pDX, IDC_PREVIEW, m_bPreview);
	DDX_Text(pDX, IDC_FONT_SIZE, m_uFontSize);
	DDX_Control(pDX, IDC_COLORVIEW, m_ColorView);
	DDX_Text(pDX, IDC_X, m_x);
	DDX_Text(pDX, IDC_Y, m_y);
	DDX_Check(pDX, IDC_SHOKE, m_shoke);
	DDX_Text(pDX, IDC_TRANSV, m_iTransV);
}

BEGIN_MESSAGE_MAP(CMyStatic, CStatic)
	ON_WM_PAINT()
END_MESSAGE_MAP()


BEGIN_MESSAGE_MAP(plugMainDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_FONT_TYPE, &plugMainDlg::OnCbnSelchangeFontType)
	ON_EN_CHANGE(IDC_CONTENT, &plugMainDlg::OnEnChangeContent)
	ON_EN_CHANGE(IDC_FONT_SIZE, &plugMainDlg::OnEnChangeFontSize)
	ON_BN_CLICKED(IDC_PREVIEW, &plugMainDlg::OnBnClickedPreview)
	ON_BN_CLICKED(IDC_SEL_COLOR, &plugMainDlg::OnBnClickedSelColor)
	ON_BN_CLICKED(IDOK, &plugMainDlg::OnBnClickedOk)
	ON_EN_CHANGE(IDC_X, &plugMainDlg::OnBnClickedPreview)
	ON_EN_CHANGE(IDC_Y, &plugMainDlg::OnBnClickedPreview)
	ON_BN_CLICKED(IDC_SHOKE, &plugMainDlg::OnBnClickedShoke)
	ON_EN_CHANGE(IDC_TRANSV, &plugMainDlg::OnEnChangeTransv)
END_MESSAGE_MAP()

BOOL plugMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	plugOS os;
	os.GatherFontfiles(m_fontInfoList);

	for each( const plugOsFontInfo& info in m_fontInfoList)
	{
		m_FontTypeCtl.AddString( info.fontName.c_str() );
	}

	if (!m_fontInfoList.empty())
	{
		m_FontTypeCtl.SetCurSel(0);
	}

	ResetFont();

	m_ColorView.setColor(m_dwColor);

	RefreshText();


	return TRUE;
}

void plugMainDlg::OnCbnSelchangeFontType()
{
	UpdateData();

	ResetFont();

	RefreshText();
}

__forceinline void blendSimpleFont(ntPlugPix* dst, unsigned char ucAlpha, unsigned int color )
{
	unsigned char ucInvAlpha = 0xFF-ucAlpha;
	dst->r= (ucAlpha* ((color>>16)&0xFF) +dst->r*ucInvAlpha)>>8;
	dst->g= (ucAlpha* ((color>>8)&0xFF) +dst->g*ucInvAlpha)>>8;
	dst->b= (ucAlpha* (color&0xFF) +dst->b*ucInvAlpha)>>8;
	dst->a= max(ucAlpha, dst->a);
}


__forceinline bool drawFontChar(ntPlugPix* adst, int canvasW, int canvasH,
								unsigned char* asrc, int textureSrcW, int textureSrcH,
								int drawX, int drawY, unsigned int color)
{
	if ( drawX >= canvasW || drawY >= canvasH )
	{
		return false;
	}

	int texDrawW = drawX+textureSrcW> canvasW ? canvasW-drawX: textureSrcW;
	int texDrawH = drawY+textureSrcH> canvasH ? canvasH-drawY: textureSrcH;
	int texDrawX = 0;
	int texDrawY = 0;

	if (drawX < 0 )
	{
		texDrawX = -drawX;
		if ( texDrawX > textureSrcW )
		{
			return false;
		}
	
		texDrawW -= texDrawX;
		drawX= 0;		
	}

	if (drawY < 0)
	{
		texDrawY = -drawY;
		if ( texDrawY > textureSrcH )
		{
			return false;
		}		
		
		texDrawH -= texDrawY;
		drawY = 0;
	}

	for(int y=0; y<texDrawH; ++y)
	{
		unsigned char* pSrc= asrc + ( y + texDrawY) * textureSrcW + texDrawX;
		ntPlugPix* pDest= adst + (y+drawY)*canvasW + drawX;

		for(int x=0; x<texDrawW; ++x)
		{
			unsigned char uc = *pSrc;
			uc = ( ((color>>24)&0xFF) * uc ) >> 8;
			blendSimpleFont(pDest, uc, color);
			++pDest;
			++pSrc;
		}
	}

	return true;
}

void plugMainDlg::OnEnChangeContent()
{
	UpdateData();

	RefreshText();
}

void plugMainDlg::ResetFont()
{
	unsigned int iCurSel= m_FontTypeCtl.GetCurSel();

	if (m_freetypeFont)
	{
		delete m_freetypeFont;
		m_freetypeFont= NULL;
	}

	if (iCurSel >= m_fontInfoList.size())
	{
		return;
	}

	if (m_uFontSize < 5)
	{
		return;
	}

	const char* sDir= getenv("windir");
	if (!sDir)
	{
		return;
	}

	char path[256];
	sprintf_s(path, "%s\\Fonts\\%s", sDir, m_fontInfoList[iCurSel].pathName.c_str());

	m_freetypeFont= new GeFontFreeTypeImpl(
		m_fontInfoList[iCurSel].fontName,
		path,
		m_uFontSize);

	if (! m_freetypeFont->Create())
	{
		delete m_freetypeFont;
		m_freetypeFont = 0;
	}
}

void plugMainDlg::OnEnChangeFontSize()
{
	UpdateData();

	ResetFont();

	RefreshText();
}

void plugMainDlg::OnBnClickedPreview()
{
	UpdateData();

	RefreshText();
}

void plugMainDlg::RefreshText()
{
	m_pParam->restoreSource();

	if ( !m_Content.IsEmpty() && m_bPreview && m_freetypeFont)
	{
		Process(m_pParam->pSource);
	}

	m_pParam->draw();
}

void plugMainDlg::OnBnClickedSelColor()
{
	CColorDialog dlg;
	if (dlg.DoModal()!= IDOK)
	{
		return;
	}

	m_dwColor = dlg.GetColor();
	m_ColorView.setColor(m_dwColor);

	RefreshText();
}

void CMyStatic::OnPaint()
{
	CPaintDC dc(this);
	RECT rect;
	GetClientRect(&rect);
	dc.FillSolidRect(&rect, m_DrawColor);
}


void CMyStatic::setColor( DWORD dwColor )
{
	m_DrawColor = dwColor;
	Invalidate(TRUE);
}

void plugMainDlg::OnBnClickedOk()
{
	unsigned int w = m_pParam->pSource->m_uiWidth;
	unsigned int h = m_pParam->pSource->m_uiHeight;
	m_pParam->pAllFunc(m_pParam->pOutput, w, h);

	if (m_bPreview)
	{
		(*m_pParam->pOutput)->copyFrom( m_pParam->pSource );
	}
	else
	{
		Process( *m_pParam->pOutput );
	}

	OnOK();
}

void plugMainDlg::Process(ntPlugPixData* pData)
{
	if (!m_freetypeFont || !pData)
	{
		return;
	}

	m_iTransV > 100 ? m_iTransV = 100 : 0;
	m_iTransV < 0 ? m_iTransV = 0 : 0;
	DWORD dwTrans= ( ( m_iTransV* 0xFF / 100 ) << 24 );

	std::wstring wstr;
	GeUnicodeHelper::ConvertStringToWString(std::string((LPCSTR)m_Content), wstr);
	DWORD dwColor = (m_dwColor&0x00FFFFFF) | dwTrans;

	static int array[]= 
	{
		-1, -1,
		 0, -1,
		 1, -1,

		-1,  0,
		 1,  0,

		-1,  1,
		 0,  1,
		 1,  1,
	};

	if (m_shoke)
	{
		DWORD v2 = (0x0033333333) | dwTrans;

		int count = sizeof(array)/sizeof(array[0]);
		for( int i=0; i<count/2; ++i)
		{
			DrawFreeText(wstr, pData, m_x+array[i*2+0],
				m_y+array[i*2+1], v2);
		}
	}

	DrawFreeText(wstr, pData, m_x, m_y, dwColor);
}

void plugMainDlg::DrawFreeText(std::wstring &wstr, 
							   ntPlugPixData* pData, 
							   int x, int y,							   
							   DWORD dwColor )
{
	unsigned int fontHeight= m_freetypeFont->GetFontHeight();

	unsigned int drawX=x, drawY= y;
	for(size_t i=0; i< wstr.length(); ++i)
	{
		wchar_t c = wstr[i];
		if ( c < 32 )
		{
			if ( c == L'\n')
			{
				drawY += fontHeight;
				drawX = x;
			}
			continue;
		}

		const GeCharGlyph* pGlyph= m_freetypeFont->MakeChar(c);
		drawFontChar(
			pData->m_pPixelData,
			pData->m_uiWidth, 
			pData->m_uiHeight,
			pGlyph->pbuff, 
			pGlyph->w,
			pGlyph->h, 
			drawX+ pGlyph->sOffsetX,
			drawY+ fontHeight - pGlyph->sOffsetY, 
			dwColor);

		drawX += pGlyph->sAdvance + 4;
	}
}

void plugMainDlg::OnBnClickedShoke()
{
	UpdateData();

	RefreshText();
}

void plugMainDlg::OnEnChangeTransv()
{
	UpdateData();

	RefreshText();
}
