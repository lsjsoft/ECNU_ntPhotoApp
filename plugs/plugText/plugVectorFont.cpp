#include "stdafx.h"
#include "plugVectorFont.h"
#include "plutFontMgr.h"
#include "plugDefine.h"


GeFontFreeTypeImpl::GeFontFreeTypeImpl(	const std::string& rkName, 
									   const std::string& rkFilename, 
									   unsigned int uiSize)
									   : m_pkFontFace(NULL)
									   , m_kFilename(rkFilename)
									   , m_kFontName(rkName)
									   , m_uiSize(uiSize)
{
}

GeFontFreeTypeImpl::~GeFontFreeTypeImpl()
{
	Release();

	if (m_pkFontFace)
	{
		GeFTFontFaceManager::GetInstance().DestroyFontFace(m_pkFontFace);
		m_pkFontFace = 0;
	}
}

bool GeFontFreeTypeImpl::Create()
{
	m_pkFontFace = GeFTFontFaceManager::GetInstance().CreateFontFace(
		m_kFilename, m_uiSize);

	if (!m_pkFontFace)
	{
		return false;
	}

	return true;
}

const GeCharGlyph* GeFontFreeTypeImpl::MakeChar(wchar_t wcChar)
{
	const GeCharGlyph* p = GetCharGlyph(wcChar);

	if (!p)
	{
		p = CreateCharGlyphAt(wcChar);
		m_Glyphs[wcChar] = p;
	}

	return p;
}

const GeCharGlyph* GeFontFreeTypeImpl::GetCharGlyph(wchar_t wcChar) const
{
	FtCharGlyphMapIt it = m_Glyphs.find(wcChar);
	return m_Glyphs.end() == it ? NULL : it->second;
}

GeFTCharGlyph* GeFontFreeTypeImpl::CreateCharGlyphAt(wchar_t wcChar)
{
	GeFTCharGlyph* pkCharGlyph = new GeFTCharGlyph;
	if (!pkCharGlyph)
	{
		return NULL;
	}

	pkCharGlyph->wcChar = wcChar;

	GlyphCharInfo CharInfo;
	if (m_pkFontFace->GetCharGlyphData(wcChar, CharInfo))
	{
		pkCharGlyph->w = CharInfo.usWidth;
		pkCharGlyph->h = CharInfo.usHeight;
		pkCharGlyph->sOffsetX = CharInfo.sOffsetX;
		pkCharGlyph->sOffsetY = CharInfo.sOffsetY;
		pkCharGlyph->sAdvance = CharInfo.sAdvance;
		pkCharGlyph->pbuff= new unsigned char[
			pkCharGlyph->w * pkCharGlyph->h ];

		GlyphFullInfo Info;
		Info.uiWidth = pkCharGlyph->w;
		Info.uiHeight = pkCharGlyph->h;
		Info.pucBuffer = pkCharGlyph->pbuff;
		m_pkFontFace->FullCharGlyphData(wcChar, Info);
	}

	return pkCharGlyph;
}

void GeFontFreeTypeImpl::Release()
{
	for (FtCharGlyphMapIt it = m_Glyphs.begin();
		it!= m_Glyphs.end(); ++it)
	{
		const GeCharGlyph* p= it->second;
		delete []p->pbuff;
		delete p;
	}
	m_Glyphs.clear();
}

unsigned int GeFontFreeTypeImpl::GetFontHeight()
{
	return m_pkFontFace->GetFontHeight();
}

