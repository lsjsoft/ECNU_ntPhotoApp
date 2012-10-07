#pragma once

class GeFTFontFaceManager
{
public:
	static GeFTFontFaceManager& GetInstance();

	GeFTFontFaceManager();
	~GeFTFontFaceManager();

	IFreeTypeFontFace* CreateFontFace(const std::string& rkFontFaceName, 
		unsigned int uiFontSize);

	void DestroyFontFace(IFreeTypeFontFace* pkFontFace);

	FT_Library GetFTLibrary() const { return m_pkFTLibrary; }

private:
	FT_Library m_pkFTLibrary;

	struct FontFaceInfo
	{
		std::string kFontFaceName;
		FT_Face pkFace;
		unsigned int uiRefCount;
	};

	typedef std::vector<FontFaceInfo> FontFaceInfoVector;
	FontFaceInfoVector m_kFontFaceInfoVector;
};
