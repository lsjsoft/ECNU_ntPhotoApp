#pragma once
#include <string>
#include <vector>

struct plugOsFontInfo
{
	std::string fontName;
	std::string pathName;
};

typedef std::vector<plugOsFontInfo> plugOsFontInfos;

class plugOS
{
public:
	plugOS(void);
	~plugOS(void);

	unsigned int GatherFontfiles(plugOsFontInfos& Fonts);

protected:
	LONG GetNameValue(HKEY key, LPCTSTR subkey, LPCTSTR valuename, LPTSTR retdata);
	LONG GetNextNameValue(HKEY key, LPCTSTR subkey, LPTSTR szName, LPTSTR szData);
	BOOL GetWinVer(CString& strVersion, int *nVersion);
};
