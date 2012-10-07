#include "StdAfx.h"
#include "plugOS.h"

plugOS::plugOS(void)
{
}

plugOS::~plugOS(void)
{
}



#define WUNKNOWNSTR	_T("unknown Windows version")

#define W95STR		_T("Windows 95")
#define W95SP1STR	_T("Windows 95 SP1")
#define W95OSR2STR	_T("Windows 95 OSR2")
#define W98STR		_T("Windows 98")
#define W98SP1STR	_T("Windows 98 SP1")
#define W98SESTR	_T("Windows 98 SE")
#define WMESTR		_T("Windows ME")

#define WNT351STR	_T("Windows NT 3.51")
#define WNT4STR		_T("Windows NT 4")
#define W2KSTR		_T("Windows 2000")
#define WXPSTR		_T("Windows XP")
#define W7STR		_T("Windows 7")

#define WCESTR		_T("Windows CE")


#define WUNKNOWN	0

#define W9XFIRST	1
#define W95			1
#define W95SP1		2
#define W95OSR2		3
#define W98			4
#define W98SP1		5
#define W98SE		6
#define WME			7
#define W9XLAST		99

#define WNTFIRST	101
#define WNT351		101
#define WNT4		102
#define W2K			103
#define WXP			104
#define W7			105
#define WNTLAST		199


#define WCEFIRST	201
#define WCE			201
#define WCELAST		299

// from winbase.h
#ifndef VER_PLATFORM_WIN32s
#define VER_PLATFORM_WIN32s             0
#endif
#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS      1
#endif
#ifndef VER_PLATFORM_WIN32_NT
#define VER_PLATFORM_WIN32_NT           2
#endif
#ifndef VER_PLATFORM_WIN32_CE
#define VER_PLATFORM_WIN32_CE           3
#endif


/*
    This table has been assembled from Usenet postings, personal
    observations, and reading other people's code.  Please feel
    free to add to it or correct it.


         dwPlatFormID  dwMajorVersion  dwMinorVersion  dwBuildNumber
95             1              4               0             950
95 SP1         1              4               0        >950 && <=1080
95 OSR2        1              4             <10           >1080
98             1              4              10            1998
98 SP1         1              4              10       >1998 && <2183
98 SE          1              4              10          >=2183
ME             1              4              90            3000

NT 3.51        2              3              51
NT 4           2              4               0            1381
2000           2              5               0            2195
XP             2              5               1

CE             3

*/

///////////////////////////////////////////////////////////////////////////////
// GetWinVer
BOOL plugOS::GetWinVer(CString& strVersion, int *nVersion)
{
	strVersion = WUNKNOWNSTR;
	*nVersion = WUNKNOWN;

	OSVERSIONINFO osinfo;
	osinfo.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);

	if (!GetVersionEx(&osinfo))
		return FALSE;

	DWORD dwPlatformId   = osinfo.dwPlatformId;
	DWORD dwMinorVersion = osinfo.dwMinorVersion;
	DWORD dwMajorVersion = osinfo.dwMajorVersion;
	DWORD dwBuildNumber  = osinfo.dwBuildNumber & 0xFFFF;	// Win 95 needs this
	
	if ((dwPlatformId == VER_PLATFORM_WIN32_WINDOWS) && (dwMajorVersion == 4))
	{
		if ((dwMinorVersion < 10) && (dwBuildNumber == 950))
		{
			strVersion = W95STR;
			*nVersion = W95;
		}
		else if ((dwMinorVersion < 10) && 
				((dwBuildNumber > 950) && (dwBuildNumber <= 1080)))
		{
			strVersion = W95SP1STR;
			*nVersion = W95SP1;
		}
		else if ((dwMinorVersion < 10) && (dwBuildNumber > 1080))
		{
			strVersion = W95OSR2STR;
			*nVersion = W95OSR2;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber == 1998))
		{
			strVersion = W98STR;
			*nVersion = W98;
		}
		else if ((dwMinorVersion == 10) && 
				((dwBuildNumber > 1998) && (dwBuildNumber < 2183)))
		{
			strVersion = W98SP1STR;
			*nVersion = W98SP1;
		}
		else if ((dwMinorVersion == 10) && (dwBuildNumber >= 2183))
		{
			strVersion = W98SESTR;
			*nVersion = W98SE;
		}
		else if (dwMinorVersion == 90)
		{
			strVersion = WMESTR;
			*nVersion = WME;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_NT)
	{
		if ((dwMajorVersion == 3) && (dwMinorVersion == 51))
		{
			strVersion = WNT351STR;
			*nVersion = WNT351;
		}
		else if ((dwMajorVersion == 4) && (dwMinorVersion == 0))
		{
			strVersion = WNT4STR;
			*nVersion = WNT4;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 0))
		{
			strVersion = W2KSTR;
			*nVersion = W2K;
		}
		else if ((dwMajorVersion == 5) && (dwMinorVersion == 1))
		{
			strVersion = WXPSTR;
			*nVersion = WXP;
		}
		else /// if ((dwMajorVersion == 6) && (dwMinorVersion == 1))
		{
			strVersion = W7STR;
			*nVersion = W7;
		}
	}
	else if (dwPlatformId == VER_PLATFORM_WIN32_CE)
	{
		strVersion = WCESTR;
		*nVersion = WCE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetNameValue
LONG plugOS::GetNameValue(HKEY key, LPCTSTR subkey, LPCTSTR valuename, LPTSTR retdata)
{
	ASSERT(subkey && subkey[0] != 0);
	ASSERT(valuename);
	ASSERT(retdata);

	HKEY hkey;
	LONG retval = RegOpenKeyEx(key, subkey, 0, KEY_QUERY_VALUE, &hkey);

	*retdata = 0;

	if (retval == ERROR_SUCCESS) 
	{
		TRACE(_T("RegOpenKeyEx ok\n"));
		DWORD datasize = MAX_PATH;
		BYTE data[MAX_PATH];
		DWORD type = 0;

		retval = RegQueryValueEx(hkey, valuename, NULL, &type, data, &datasize);
		if (retval == ERROR_SUCCESS) 
		{
			lstrcpy(retdata, (const char *)data);
			TRACE(_T("RegQueryValueEx ok\n"));
		}
		else
		{
			TRACE(_T("RegQueryValueEx failed\n"));
		}
		RegCloseKey(hkey);
	}
	else
	{
		TRACE(_T("RegOpenKeyEx failed\n"));
	}

	return retval;
}

///////////////////////////////////////////////////////////////////////////////
// GetNextNameValue
LONG plugOS::GetNextNameValue(HKEY key, LPCTSTR subkey, LPTSTR szName, LPTSTR szData)
{
	static HKEY hkey = NULL;
	static DWORD dwIndex = 0;
	LONG retval;

	if (subkey == NULL && szName == NULL && szData == NULL)
	{
		TRACE(_T("closing key\n"));
		if (hkey)
			RegCloseKey(hkey);
		hkey = NULL;
		return ERROR_SUCCESS;
	}

	if (subkey && subkey[0] != 0)
	{
		retval = RegOpenKeyEx(key, subkey, 0, KEY_ALL_ACCESS, &hkey);
		if (retval != ERROR_SUCCESS)
		{
			TRACE(_T("RegOpenKeyEx failed\n"));
			return retval;
		}
		else
		{
			TRACE(_T("RegOpenKeyEx ok\n"));
		}
		dwIndex = 0;
	}
	else
	{
		dwIndex++;
	}

	ASSERT(szName != NULL && szData != NULL);

	*szName = 0;
	*szData = 0;

	char szValueName[MAX_PATH];
	DWORD dwValueNameSize = sizeof(szValueName)-1;
	BYTE szValueData[MAX_PATH];
	DWORD dwValueDataSize = sizeof(szValueData)-1;
	DWORD dwType = 0;

	retval = RegEnumValue(hkey, dwIndex, szValueName, &dwValueNameSize, NULL, 
		&dwType, szValueData, &dwValueDataSize);
	if (retval == ERROR_SUCCESS) 
	{
		TRACE(_T("szValueName=<%s>  szValueData=<%s>\n"), szValueName, szValueData);
		lstrcpy(szName, (char *)szValueName);
		lstrcpy(szData, (char *)szValueData);
	}
	else
	{
		TRACE(_T("RegEnumKey failed\n"));
	}

	return retval;
}


unsigned int plugOS::GatherFontfiles( plugOsFontInfos& Fonts )
{
	_TCHAR szName[2 * MAX_PATH];
	_TCHAR szData[2 * MAX_PATH];

	int nVersion;
	CString strVersion;
	GetWinVer(strVersion, &nVersion);
	TRACE(_T("strVersion=%s\n"), strVersion);

	CString strFont;
	if ((nVersion >= WNTFIRST) && (nVersion <= WNTLAST)) 
	{
		strFont = _T("Software\\Microsoft\\Windows NT\\CurrentVersion\\Fonts");
	}
	else
	{
		strFont = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Fonts");
	}

	BOOL bResult = FALSE;

	while (GetNextNameValue(HKEY_LOCAL_MACHINE, strFont, szName, szData) == ERROR_SUCCESS)
	{
		plugOsFontInfo info;
		info.fontName= szName;
		info.pathName= szData;
		Fonts.push_back(info);
		strFont.Empty();
	}
	GetNextNameValue(HKEY_LOCAL_MACHINE, NULL, NULL, NULL);

	return (unsigned int)Fonts.size();
}
