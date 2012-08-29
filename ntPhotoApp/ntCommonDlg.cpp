#include "StdAfx.h"
#include "ntCommonDlg.h"
#include "ximage.h"

struct ntDocType
{
	int nID;
	bool bRead;
	bool bWrite;
	const char* description;
	const char* ext;
};

ntDocType doctypes[] =
{
	{ 
		-1, 
		true, 
		true, 
		"all Files",
		"*.bmp;*.gif;*.jpg;*.jpeg;*.png;*.ico;*.tif;"
		"*.tiff;*.tga;*.pcx;*.wbmp;*.wmf;*.emf;*.j2k;"
		"*.jp2;*.jbg;*.j2c;*.jpc;*.pgx;*.pnm;*.pgm;"
		"*.ppm;*.ras;*.mng;*.jng;*.ska;*.nef;*.crw;"
		"*.cr2;*.mrw;*.raf;*.erf;*.3fr;*.dcr;*.raw;"
		"*.dng;*.pef;*.x3f;*.arw;*.sr2;*.mef;*.orf",
	},

	{ 
		CXIMAGE_FORMAT_JPG, 
		true, 
		true, 
		"JPG files",
		"*.jpg;*.jpeg",
	},

	{
		CXIMAGE_FORMAT_TGA, 
		true, 
		true, 
		"TGA files",
		"*.tga",
	},
};

//////////////////////////////////////////////////////////////////////////////
int ntCommonDlg::GetIndexFromType(int nDocType, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<ntGetCount(doctypes);i++)
	{
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite)
		{
			if (doctypes[i].nID == nDocType) return nCnt;
			nCnt++;
		}
	}
	return -1;
}
//////////////////////////////////////////////////////////////////////////////
int ntCommonDlg::GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog)
{
	int nCnt = 0;
	for (int i=0;i<ntGetCount(doctypes);i++)
	{
		if (bOpenFileDialog ? doctypes[i].bRead : doctypes[i].bWrite)
		{
			if (nCnt == nIndex)
				//              return i; // PJO - Buglet ?
				return doctypes[i].nID;
			nCnt++;
		}
	}
	ASSERT(FALSE);
	return -1;
}
//////////////////////////////////////////////////////////////////////////////
CString ntCommonDlg::GetExtFromType(int nDocType)
{
	for (int i=0;i<ntGetCount(doctypes);i++)
	{
		if (doctypes[i].nID == nDocType)
			return doctypes[i].ext;
	}
	return CString(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
CString ntCommonDlg::GetDescFromType(int nDocType)
{
	for (int i=0;i<ntGetCount(doctypes);i++)
	{
		if (doctypes[i].nID == nDocType)
			return doctypes[i].description;
	}
	return CString(_T(""));
}
//////////////////////////////////////////////////////////////////////////////
BOOL ntCommonDlg::GetWritableType(int nDocType)
{
	for (int i=0;i<ntGetCount(doctypes);i++)
	{
		if (doctypes[i].nID == nDocType)
			return doctypes[i].bWrite;
	}
	return FALSE;
}
//////////////////////////////////////////////////////////////////////////////
CString ntCommonDlg::GetFileTypes(BOOL bOpenFileDialog)
{
	CString str;
	for (int i=0;i<ntGetCount(doctypes);i++)
	{
		if (bOpenFileDialog && doctypes[i].bRead)
		{
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		} 
		else if (!bOpenFileDialog && doctypes[i].bWrite)
		{
			str += doctypes[i].description;
			str += (TCHAR)NULL;
			str += doctypes[i].ext;
			str += (TCHAR)NULL;
		}
	}
	return str;
}

//////////////////////////////////////////////////////////////////////////////
BOOL ntCommonDlg::PromptForFileName(CString& fileName, UINT, 
	DWORD dwFlags, BOOL bOpenFileDialog, int* pType)
{
	CFileDialog dlgFile(bOpenFileDialog);
	CString title;
	if (bOpenFileDialog) 
	{
		title=_T("Open image file"); 
	}
	else 
	{
		title=_T("Save image file");
	}

	dlgFile.m_ofn.Flags |= dwFlags;

	int nDocType = (pType != NULL) ? *pType : CXIMAGE_FORMAT_BMP;
	if (nDocType==0) 
	{
		nDocType=1;
	}

	int nIndex = GetIndexFromType(nDocType, bOpenFileDialog);
	if (nIndex == -1) 
	{
		nIndex = 0;
	}

	dlgFile.m_ofn.nFilterIndex = nIndex +1;

	CString strDefExt = GetExtFromType(nDocType).Mid(2,3);
	dlgFile.m_ofn.lpstrDefExt = strDefExt;

	CString strFilter = GetFileTypes(bOpenFileDialog);
	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	BOOL bRet = (dlgFile.DoModal() == IDOK) ? TRUE : FALSE;
	fileName.ReleaseBuffer();
	if (bRet)
	{
		if (pType != NULL)
		{
			int nIndex = (int)dlgFile.m_ofn.nFilterIndex - 1;
			ASSERT(nIndex >= 0);
			*pType = GetTypeFromIndex(nIndex, bOpenFileDialog);
		}
	}
	return bRet;
}

