#pragma once
#include "ntGdiObject.h"

class ntDibBitmap: public ntGdiObject
{
public:
	ntDibBitmap(void);
	~ntDibBitmap(void);

	bool create(HDC dc, unsigned int uiWidth, unsigned int uiHeight);

	void* getData() { return m_pData; }
	
	unsigned int getHeight() const { return m_uiHeight; }

	unsigned int getWidth() const { return m_uiWidth; }

protected:
	void* m_pData;
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
};
