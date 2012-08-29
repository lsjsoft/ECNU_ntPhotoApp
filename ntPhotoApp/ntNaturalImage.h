#pragma once
#include "ntBuffer.h"

class CxImage;

class ntNaturalImage
{
public:
	ntNaturalImage(void);
	~ntNaturalImage(void);

	bool load(const std::string& rkImagePathname);

	bool save(const std::string& rkImagePathname, void* pData, 
		unsigned int uiWidth, unsigned int uiHeight,
		int iFormat);

	void clear();

	unsigned int getWidth() const { return m_uiWidth; }

	unsigned int getHeight() const { return m_uiHeight; }

	ntBufferPtr getBuffer() const { return m_spBuffer; }

protected:

	bool loadCxImage(CxImage* pCxImage);

	bool createBuffer(unsigned int uiWidth, unsigned int uiHeight);

protected:
	unsigned int m_uiWidth;
	unsigned int m_uiHeight;
	ntBufferPtr m_spBuffer;
};
