#include "StdAfx.h"
#include "ntDibBitmap.h"

ntDibBitmap::ntDibBitmap(void)
{
}

ntDibBitmap::~ntDibBitmap(void)
{
}

bool ntDibBitmap::create( HDC dc, unsigned int uiWidth, unsigned int uiHeight )
{
	reset();

	BITMAPINFO	bmi;
	bmi.bmiHeader.biSize = sizeof(bmi.bmiHeader);
	bmi.bmiHeader.biWidth = uiWidth;
	bmi.bmiHeader.biHeight = -(int)uiHeight;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = 0;
	bmi.bmiHeader.biSizeImage = 0;
	bmi.bmiHeader.biXPelsPerMeter = 0;
	bmi.bmiHeader.biYPelsPerMeter = 0;
	bmi.bmiHeader.biClrImportant = 0;
	bmi.bmiHeader.biClrUsed = 0;

	m_hObject = CreateDIBSection( dc, &bmi, DIB_RGB_COLORS, (void**)&m_pData, NULL, 0 );
	if(!m_hObject)
	{
		return FALSE;
	}

	m_uiHeight = uiHeight;
	m_uiWidth = uiWidth;

	return TRUE;
}
