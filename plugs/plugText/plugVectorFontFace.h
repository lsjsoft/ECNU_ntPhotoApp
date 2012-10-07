#pragma once

class GeFreeTypeVectorFace: public IFreeTypeFontFace
{
	friend class GeFTFontFaceManager;

private:
	GeFreeTypeVectorFace(FT_Face pkFace, unsigned int uiFontSize);
	~GeFreeTypeVectorFace();

public:
	virtual FT_Face GetFontFace() const
	{
		return m_pkFace;
	}

	virtual unsigned int GetBaseLine() const
	{
		return m_uiBaseLine;
	}

	virtual unsigned int GetFontHeight() const
	{
		return m_uiFontHeight;
	}

	virtual unsigned int GetLineSpacing() const 
	{
		return m_uiLineSpacing;
	}

	virtual unsigned int GetWideCharGlyphSize()
	{
		return m_uiWideCharGlyphSize;
	}

	virtual bool GetCharGlyphData(wchar_t wcChar, GlyphCharInfo& Info)
	{
		return GetCharGlyphInfo(wcChar, Info.usWidth, Info.usHeight, Info.sOffsetX,
			Info.sOffsetY, Info.sAdvance);
	}

	virtual bool FullCharGlyphData(wchar_t wcChar, const GlyphFullInfo& Info)
	{
		return GetCharGlyphData( wcChar, Info.uiWidth, Info.uiHeight, 1, Info.pucBuffer);
	}

private:
	void SetFontSize();
	bool LoadCharGlyph(wchar_t wcChar);

	bool GetCharGlyphInfo(wchar_t wcChar, unsigned short& usWidth, 
		unsigned short& usHeight, short& sOffsetX, short& sOffsetY, short& sAdvance);

	bool GetCharGlyphData(wchar_t wcChar, unsigned int uiWidth, unsigned int uiHeight, 
		unsigned int uiBPP, unsigned char* pucBuffer);

	FT_Face m_pkFace;

	unsigned int m_uiFontSize;
	unsigned int m_uiBaseLine;
	unsigned int m_uiFontHeight;
	unsigned int m_uiLineSpacing;
	unsigned int m_uiWideCharGlyphSize;

	wchar_t m_wcChar;
	bool m_bIsLoadGlyphData;

	typedef std::map<wchar_t, FT_Glyph> GlyphMap;
	GlyphMap m_kGlyphMap;
};