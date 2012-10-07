#include "StdAfx.h"
#include "plutFontMgr.h"
#include "plugVectorFont.h"
#include "plugVectorFontFace.h"

#pragma comment(lib, "../../lib/freetype2410MT.lib")

GeFTFontFaceManager& GeFTFontFaceManager::GetInstance()
{
	static GeFTFontFaceManager kFTFontFaceManager;
	return kFTFontFaceManager;
}

GeFTFontFaceManager::GeFTFontFaceManager()
{
	int iError = FT_Init_FreeType(&m_pkFTLibrary);
	assert(!iError);
}

GeFTFontFaceManager::~GeFTFontFaceManager()
{
	assert(m_kFontFaceInfoVector.empty());
	FT_Done_FreeType(m_pkFTLibrary);
}

IFreeTypeFontFace* GeFTFontFaceManager::CreateFontFace(
	const std::string& rkFontFaceName, unsigned int uiFontSize)
{
	assert(m_pkFTLibrary);

	FT_Face pkFace = NULL;
	for (unsigned int i = 0; i < m_kFontFaceInfoVector.size(); ++i)
	{
		if (m_kFontFaceInfoVector[i].kFontFaceName == rkFontFaceName)
		{
			pkFace = m_kFontFaceInfoVector[i].pkFace;
			++m_kFontFaceInfoVector[i].uiRefCount;
		}
	}

	if (!pkFace)
	{
 		int iError = FT_New_Face(m_pkFTLibrary, rkFontFaceName.c_str(), 0, &pkFace);
		assert(!iError);

		FontFaceInfo kFontFaceInfo = {};
		kFontFaceInfo.kFontFaceName = rkFontFaceName;
		kFontFaceInfo.pkFace = pkFace;
		kFontFaceInfo.uiRefCount = 1;
		m_kFontFaceInfoVector.push_back(kFontFaceInfo);
	}

	if(!pkFace)
	{
		return 0;
	}

	IFreeTypeFontFace* pkFontFace= new GeFreeTypeVectorFace(pkFace, uiFontSize);
	if(!pkFontFace)
	{
		return 0;
	}

	return pkFontFace;
}

void GeFTFontFaceManager::DestroyFontFace(IFreeTypeFontFace* pkFontFace)
{
	if (m_pkFTLibrary == NULL || pkFontFace == NULL)
	{
		return;
	}

	FT_Face pkFace = pkFontFace->GetFontFace();

	for (unsigned int i = 0; i < m_kFontFaceInfoVector.size(); ++i)
	{
		if (m_kFontFaceInfoVector[i].pkFace == pkFace)
		{
			if (--m_kFontFaceInfoVector[i].uiRefCount == 0)
			{
				FT_Done_Face(pkFace);
				m_kFontFaceInfoVector.erase(m_kFontFaceInfoVector.begin() + i);
				delete pkFontFace;
				return;
			}			
		}
	}

	delete pkFontFace;

}
