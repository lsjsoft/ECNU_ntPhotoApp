#include "StdAfx.h"
#include "ntPlugBaseService.h"

ntPlugBaseService::ntPlugBaseService(void)
{
}

ntPlugBaseService::~ntPlugBaseService(void)
{
}

bool ntPlugBaseService::makeGray( ntTexture32Ptr tex )
{
	if(!tex)
	{
		return false;
	}

	ntPixel32* pBuffer= tex->getBuffer();
	unsigned int uiHeight= tex->getHeight();
	unsigned int uiWidth= tex->getWidth();

	for( unsigned int i=0; i< uiWidth * uiHeight; ++i, ++pBuffer)
	{
		pBuffer->red = pBuffer->green = pBuffer->blue 
			= pBuffer->getLuminance();
	}

	tex->setModify();

	return true;
}

bool ntPlugBaseService::makeInvert( ntTexture32Ptr tex )
{
	if(!tex)
	{
		return false;
	}

	ntPixel32* pBuffer= tex->getBuffer();
	unsigned int uiHeight= tex->getHeight();
	unsigned int uiWidth= tex->getWidth();

	for(unsigned int i=0; i<uiHeight * uiWidth; ++i)
	{
		pBuffer->red = 0xFF - pBuffer->red;
		pBuffer->green = 0xFF - pBuffer->green;
		pBuffer->blue = 0xFF - pBuffer->blue;
		++pBuffer;
	}

	tex->setModify();

	return true;
}

ntHistogramPtr ntPlugBaseService::getHistogram( ntTexture32Ptr tex )
{
	ntHistogramPtr histogram = ntNew ntHistogram();
	if(!tex)
	{
		return histogram;
	}

	ntPixel32* pBuffer= tex->getBuffer();
	unsigned int uiHeight= tex->getHeight();
	unsigned int uiWidth= tex->getWidth();

	for(unsigned int i=0; i<uiHeight * uiWidth; ++i)
	{
		unsigned int lu = pBuffer->getLuminance();
		histogram->r[ pBuffer->red ]++;
		histogram->g[ pBuffer->green ]++;
		histogram->b[ pBuffer->blue ]++;
		histogram->rgb[ lu ] ++;
		pBuffer++;
	}

	return histogram;
}
