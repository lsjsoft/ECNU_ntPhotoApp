#include "stdafx.h"
#include "GeUnicodeHelper.h"

UINT GeUnicodeHelper::s_iACP = 0;

void GeUnicodeHelper::ConvertStringToWString(const std::string& rkText, 
											 std::wstring& rkWText,
											 UINT iCodepage)
{
	int iTextLength = MultiByteToWideChar(iCodepage, 0, rkText.c_str(), 
		(int)rkText.length(), NULL, 0);
	wchar_t * pkTargetText 
		= (wchar_t *)alloca((iTextLength + 1) * sizeof(wchar_t));
	MultiByteToWideChar(iCodepage, 0, rkText.c_str(), 
		(int)rkText.length(), pkTargetText, iTextLength);
	pkTargetText[iTextLength] = L'\0';	
	rkWText = pkTargetText;
}

void GeUnicodeHelper::ConvertWStringToString(const std::wstring& rkWText, 
											 std::string& rkText, 
											 UINT iCodepage)
{
	int iTextLength = WideCharToMultiByte(iCodepage, 0, rkWText.c_str(), 
		(int)rkWText.length(), NULL, 0, NULL, NULL);
	char * pkTargetText 
		= (char *)alloca((iTextLength + 1) * sizeof(char));
	WideCharToMultiByte(iCodepage, 0, rkWText.c_str(), 
		(int)rkWText.length(), pkTargetText, iTextLength, NULL, NULL);
	pkTargetText[iTextLength] = '\0';	
	rkText = pkTargetText;
}

bool GeUnicodeHelper::IsSystemPageCodeGBK()
{
	if (s_iACP == 0)
	{
		s_iACP = GetACP();
	}
	return s_iACP == CP_GBK;
}
