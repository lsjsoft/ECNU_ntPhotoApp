#include "StdAfx.h"
#include "plugVectorFontFace.h"


#define UP_INT(a) (unsigned int)( (float)(a) + 0.5f)

GeFreeTypeVectorFace::GeFreeTypeVectorFace(FT_Face pkFace, unsigned int uiFontSize)
: m_pkFace(pkFace)
, m_uiFontSize(uiFontSize)
, m_wcChar(0)
{
	assert(pkFace);
	SetFontSize();

	const float fScale = m_pkFace->size->metrics.y_scale / 64.0f * (1.0f / 65536.0f);
	m_uiBaseLine = (unsigned int)(m_pkFace->ascender * fScale) + 1;
	m_uiFontHeight = (unsigned int)((m_pkFace->ascender - m_pkFace->descender) * fScale) + 1;
	m_uiLineSpacing = (unsigned int)(m_pkFace->height * fScale) + 1;

	FT_Load_Char (m_pkFace, 34255, FT_LOAD_FORCE_AUTOHINT);

	m_uiWideCharGlyphSize = (unsigned int)( (float)m_pkFace->glyph->metrics.horiAdvance / 64.0f + 2 );
}

GeFreeTypeVectorFace::~GeFreeTypeVectorFace()
{
}

bool GeFreeTypeVectorFace::GetCharGlyphInfo(wchar_t wcChar, 
											unsigned short& usWidth, 
											unsigned short& usHeight, 
											short& sOffsetX, short& sOffsetY, short& sAdvance)
{
	bool bSuccess = LoadCharGlyph(wcChar);

	if (bSuccess)
	{
		usWidth = (unsigned short)m_pkFace->glyph->bitmap.width;
		usHeight = (unsigned short)m_pkFace->glyph->bitmap.rows;
		sOffsetX = (short)m_pkFace->glyph->metrics.horiBearingX / 64;
		sOffsetY = (short)m_pkFace->glyph->metrics.horiBearingY / 64;
		sAdvance = (short)( (float)m_pkFace->glyph->metrics.horiAdvance / 64.0f + 1);
	}
	else
	{
		usWidth = 0;
		usHeight = 0;
		sOffsetX = 0;
		sOffsetY = 0;
		sAdvance = (short)( (float)m_pkFace->glyph->metrics.horiAdvance / 64.0f + 1);
	}

	return bSuccess;	
}

bool GeFreeTypeVectorFace::GetCharGlyphData(wchar_t wcChar, 
											unsigned int uiWidth, 
											unsigned int uiHeight, 
											unsigned int uiBpp, 
											unsigned char* pucBuffer)
{
	if (!m_bIsLoadGlyphData)
	{
		return false;
	}

	if (!pucBuffer)
	{
		return false;
	}

	FT_Bitmap* pkBitmap = &m_pkFace->glyph->bitmap;

	if (uiBpp == 1)
	{
		if (pkBitmap->pixel_mode == FT_PIXEL_MODE_MONO)
		{
			for (int y = 0; y < pkBitmap->rows; ++y)
			{
				unsigned char* pucSrc = pkBitmap->buffer + (y * pkBitmap->pitch);
				unsigned char* pucDst = pucBuffer + pkBitmap->width * y;
				for (int x = 0; x < pkBitmap->width; ++x)
				{
					pucDst[x] = (pucSrc[x / 8] & (0x80 >> (x & 7))) ? 0xFF : 0x00;
				}
			}
		}
		else if (pkBitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
		{
			for (int y = 0; y < pkBitmap->rows; ++y)
			{
				unsigned char* pucSrc = pkBitmap->buffer + (y * pkBitmap->pitch);
				unsigned char* pucDst = pucBuffer + pkBitmap->width * y;

				for (int x = 0; x < pkBitmap->width; ++x)
				{
					*pucDst++ = *pucSrc++;				
				}
			}
		}
	}
	else if (uiBpp == 4)
	{
		if (pkBitmap->pixel_mode == FT_PIXEL_MODE_MONO)
		{
			for (int y = 0; y < pkBitmap->rows; ++y)
			{
				unsigned char* pucSrc = pkBitmap->buffer + (y * pkBitmap->pitch);
				unsigned char* pucDst = pucBuffer + pkBitmap->width * uiBpp * y;
				for (int x = 0; x < pkBitmap->width; ++x)
				{
					pucDst[4 * x] = (pucSrc[x / 8] & (0x80 >> (x & 7))) ? 0xFF : 0x00;
					pucDst[4 * x + 1] = pucDst[4 * x];
					pucDst[4 * x + 2] = pucDst[4 * x];
					pucDst[4 * x + 3] = pucDst[4 * x];					
				}
			}
		}
		else if (pkBitmap->pixel_mode == FT_PIXEL_MODE_GRAY)
		{
			for (int y = 0; y < pkBitmap->rows; ++y)
			{
				unsigned char* pucSrc = pkBitmap->buffer + (y * pkBitmap->pitch);
				unsigned char* pucDst = pucBuffer + pkBitmap->width * uiBpp * y;

				for (int x = 0; x < pkBitmap->width; ++x)
				{
					*(pucDst    ) = *pucSrc;				
					*(pucDst + 1) = *pucSrc;				
					*(pucDst + 2) = *pucSrc;				
					*(pucDst + 3) = *pucSrc;
					pucSrc++; 
					pucDst += 4;
				}
			}
		}
	}

	return true;
}

void GeFreeTypeVectorFace::SetFontSize()
{
	FT_Set_Pixel_Sizes(m_pkFace, 0, m_uiFontSize);
}

bool GeFreeTypeVectorFace::LoadCharGlyph(wchar_t wcChar)
{
	if (m_wcChar == wcChar)
	{
		return m_bIsLoadGlyphData;
	}

	m_wcChar = wcChar;
	m_bIsLoadGlyphData = false;

	SetFontSize();

	if (0 == FT_Load_Char(m_pkFace, wcChar, FT_LOAD_FORCE_AUTOHINT))
	{
		m_bIsLoadGlyphData = true;
		FT_Render_Glyph(m_pkFace->glyph, FT_RENDER_MODE_NORMAL);
	}

	return m_bIsLoadGlyphData;
}

