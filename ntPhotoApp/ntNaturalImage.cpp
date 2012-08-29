#include "StdAfx.h"
#include "ntNaturalImage.h"
#include "ximage.h"

#pragma comment(lib, "../lib/geimage.lib")

ntNaturalImage::ntNaturalImage(void)
: m_uiWidth(0)
, m_uiHeight(0)
{
}

ntNaturalImage::~ntNaturalImage(void)
{
}

bool ntNaturalImage::load( const std::string& rkImagePathname )
{
	CxImage Image;
	if (! Image.Load(rkImagePathname.c_str()))
	{
		return false;
	}

	if (! loadCxImage( &Image ))
	{
		return false;
	}

	return true;
}

bool ntNaturalImage::loadCxImage( CxImage* pCxImage )
{
	if (!pCxImage)
	{
		return false;
	}

	CxImage& Image= *pCxImage;

	unsigned short bpp = Image.GetBpp();
	unsigned char* lpAlpha= Image.GetAlphaData();

	int iWidth= Image.GetWidth();
	int iHeight= Image.GetHeight();

	if ( bpp == 24 )
	{
		createBuffer( iWidth, iHeight );

		ntPixel32* lpData= m_spBuffer->getBuffer<ntPixel32>();
		unsigned short ucSize= bpp >> 3;

		for(int i=iHeight-1; i>=0; --i)
		{
			unsigned char* lpSrc= Image.GetBits(i);
			for(int j=0; j<iWidth; ++j)
			{
				lpData->red = lpSrc[2];
				lpData->green=lpSrc[1];
				lpData->blue= lpSrc[0];
				lpData->alpha = lpAlpha ? lpAlpha[i*iWidth+j] : 0xFF;
				lpData++;
				lpSrc+= ucSize;
			}
		}

		m_uiWidth = iWidth;
		m_uiHeight = iHeight;

		return true;
	}
	
	return false;

/*
	case 8:	// desc: 8位索引图
		{
			CreateVirtual(Image.GetWidth(), Image.GetHeight(), 32);

			CGePix32* lpData= (CGePix32*)m_pData;

			RGBQUAD* lpQuad= Image.GetPalette();

			for(int i=Image.GetHeight()-1; i>=0; --i)
			{
				BYTE* lpSrc= Image.GetBits(i);

				for(int j=0; j<Image.GetWidth(); ++j)
				{
					RGBQUAD lpQuads= lpQuad[*lpSrc];
					lpData->red		= lpQuads.rgbRed;
					lpData->green	= lpQuads.rgbGreen;
					lpData->blue	= lpQuads.rgbBlue;

					if ( (*lpSrc) == nTransIndexColor )
					{
						lpData->alpha = 0;
					}
					else
					{
						lpData->alpha = 255;
					}

					lpData++;
					lpSrc++;
				}
			}
		}
		break;

	case 4:	// desc: 4位索引图
		{
			CreateVirtual(Image.GetWidth(), Image.GetHeight(), 32);

			CGePix32* lpData= (CGePix32*)GetData();

			RGBQUAD* lpQuad = Image.GetPalette();

			for(int i=Image.GetHeight()-1; i>=0; --i)
			{
				BYTE* lpSrc= Image.GetBits(i);

				int n = 0;

				for(int j=0; j< Image.GetWidth(); ++j)
				{
					BYTE Src= *lpSrc;
					BYTE Low = (Src & 0x0f);
					BYTE Hig = (Src & 0xf0) >> 4;

					RGBQUAD lpQuads= lpQuad[Hig];
					lpData->red		= lpQuads.rgbRed;
					lpData->green	= lpQuads.rgbGreen;
					lpData->blue	= lpQuads.rgbBlue;

					if ( Hig == nTransIndexColor )
					{
						lpData->alpha = 0;
					}
					else
					{
						lpData->alpha = 255;
					}

					lpData++;
					if (++n >= Image.GetWidth()) break;

					lpQuads= lpQuad[Low];
					lpData->red		= lpQuads.rgbRed;
					lpData->green	= lpQuads.rgbGreen;
					lpData->blue	= lpQuads.rgbBlue;

					if ( Low == nTransIndexColor )
					{
						lpData->alpha = 0;
					}
					else
					{
						lpData->alpha = 255;
					}

					lpData++;
					if (++n >= Image.GetWidth()) break;

					lpSrc++;
				}
			}
		}
		break;
*/
}

bool ntNaturalImage::createBuffer( unsigned int uiWidth, unsigned int uiHeight )
{
	if (!m_spBuffer)
	{
		m_spBuffer= ntNew ntBuffer();
	}

	m_spBuffer->clear();
	m_spBuffer->allocBuffer<ntPixel32>(uiWidth*uiHeight);
	return true;
}

bool ntNaturalImage::save(const std::string& rkImagePathname, void* pData, 
						  unsigned int uiWidth, unsigned int uiHeight,
						  int iFormat)
{
	const unsigned int bpp = 4;
	unsigned int uiBytesPreLine= uiWidth * bpp;
	unsigned int uiBytesPrePixel= bpp * 8;

	CxImage Image;
	if (! Image.CreateFromArray( (BYTE*)pData, uiWidth, uiHeight, 
		uiBytesPrePixel, uiBytesPreLine, true, true))
	{
		ntLog(Image.GetLastError());
		return false;
	}

	if (! Image.Save( rkImagePathname.c_str(), iFormat ))
	{
		ntLog(Image.GetLastError());
		return false;
	}

	return true;
}
