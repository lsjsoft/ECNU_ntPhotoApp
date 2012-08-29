#pragma once

class ntCommonDlg
{
public:
	ntCommonDlg() {}
	~ntCommonDlg() {}

	BOOL PromptForFileName(CString& fileName, UINT /*nIDSTitle*/, 
		DWORD dwFlags, BOOL bOpenFileDialog, int* pType);

protected:
	int GetIndexFromType(int nDocType, BOOL bOpenFileDialog);
	int GetTypeFromIndex(int nIndex, BOOL bOpenFileDialog);
	CString GetExtFromType(int nDocType);
	CString GetDescFromType(int nDocType);
	BOOL GetWritableType(int nDocType);
	CString GetFileTypes(BOOL bOpenFileDialog);
};

