#pragma once

#define CP_GBK 936

class GeUnicodeHelper
{
public:
	static void ConvertStringToWString(const std::string& rkText, 
		std::wstring& rkWText , UINT iCodepage = CP_GBK);

	static void ConvertWStringToString(const std::wstring& rkWText, 
		std::string& rkText, UINT iCodepage = CP_GBK);

	static inline bool IsSystemPageCodeGBK();

private:
	static UINT s_iACP;
};
