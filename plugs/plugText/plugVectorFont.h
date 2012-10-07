#pragma once

struct GeCharGlyph
{
	GeCharGlyph()
	: x(0), y(0), w(0), h(0), sOffsetX(0), sOffsetY(0), sAdvance(0), pbuff(0)
	{
	}

	wchar_t wcChar;
	short x, y;
	unsigned short w, h;
	short sOffsetX;
	short sOffsetY;
	short sAdvance;
	unsigned char* pbuff;
};

typedef GeCharGlyph GeFTCharGlyph;


class GeFontFreeTypeImpl
{
public:
	GeFontFreeTypeImpl(const std::string& rkName, const std::string& rkFilename, 
		unsigned int uiSize);

	virtual ~GeFontFreeTypeImpl();

	virtual bool Create();

	virtual const GeCharGlyph* MakeChar(wchar_t wcChar);

	unsigned int GetFontHeight();

protected:

	virtual const GeCharGlyph* GetCharGlyph(wchar_t wcChar) const;

	void Release();

	GeFTCharGlyph* CreateCharGlyphAt(wchar_t wcChar);

protected:
	IFreeTypeFontFace* m_pkFontFace;

	unsigned int m_uiCurrentX;
	unsigned int m_uiCurrentY;

	float m_fOffset;

	typedef std::map<wchar_t, const GeFTCharGlyph*> FtCharGlyphMap;
	typedef FtCharGlyphMap::const_iterator FtCharGlyphMapIt;
	FtCharGlyphMap m_Glyphs;

	std::string m_kFilename;
	std::string m_kFontName;
	unsigned int m_uiSize;	
};
