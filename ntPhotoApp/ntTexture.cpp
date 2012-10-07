#include "StdAfx.h"
#include "ntTexture.h"


void ntTexture32::setPix( unsigned int x, unsigned int y, const ntPixel32& px )
{
	if (x < m_textureInfo.m_uiWidth && y < m_textureInfo.m_uiHeight )
	{
		ntPixel32* p = m_spBuffer->getBuffer<ntPixel32>();
		p[m_textureInfo.m_uiWidth * y + x ] = px;
	}
}

void ntTexture32::drawPixel( unsigned int x, unsigned int y, unsigned int color )
{
	if ( x < m_textureInfo.m_uiWidth && y < m_textureInfo.m_uiHeight)
	{
		ntPixel32* p= m_spBuffer->getBuffer<ntPixel32>();
		ntPixel32& dest= p[y*m_textureInfo.m_uiWidth+x];
		unsigned int uiDest = *((unsigned int*)&dest);
		unsigned char ucAlpha = (color>>24)&0xFF;
		unsigned char ucInvAlpha = 0xFF-ucAlpha;
		unsigned char ucDstAlpha= (uiDest>>24)&0xFF;
		ucDstAlpha = max(ucDstAlpha, ucAlpha);
		*((unsigned int*)&dest)= ntblend3(
			ucAlpha, color, ucInvAlpha, uiDest) | (ucDstAlpha << 24 );
	}
}

bool ntTexture32::bltTo( ntTexture32Ptr dest )
{
	if (!dest)
	{
		return false;
	}

	if (m_textureInfo.m_uiWidth != dest->m_textureInfo.m_uiWidth ||
		m_textureInfo.m_uiHeight != dest->m_textureInfo.m_uiHeight)
	{
		return false;
	}

	unsigned int uiSize= m_textureInfo.m_uiWidth * m_textureInfo.m_uiHeight;

	ntPixel32* psrc= m_spBuffer->getBuffer<ntPixel32>();
	ntPixel32* pdest= dest->m_spBuffer->getBuffer<ntPixel32>();

	while(uiSize != 0)
	{
		pdest->blend(psrc);
		pdest->alpha = max( pdest->alpha, psrc->alpha);
		--uiSize;
		++pdest, ++psrc;
	}

	return true;
}
