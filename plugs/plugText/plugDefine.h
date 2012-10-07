#pragma once

struct GlyphCharInfo
{
	unsigned short usWidth;
	unsigned short usHeight;
	short sOffsetX;
	short sOffsetY;
	short sAdvance;
};

struct GlyphFullInfo
{
	unsigned int uiWidth;
	unsigned int uiHeight;
	unsigned char* pucBuffer;
};

class IFontFace
{
public:
	virtual unsigned int GetBaseLine() const = 0;
	virtual unsigned int GetFontHeight() const = 0;
	virtual unsigned int GetLineSpacing() const = 0;
	virtual unsigned int GetWideCharGlyphSize() = 0;
	virtual bool GetCharGlyphData(wchar_t wcChar, GlyphCharInfo& Info) = 0;
	virtual bool FullCharGlyphData(wchar_t wcChar, const GlyphFullInfo& Info) = 0;
	virtual ~IFontFace() {};
};

class IFreeTypeFontFace: public IFontFace
{
public:
	virtual FT_Face GetFontFace() const = 0;
};
