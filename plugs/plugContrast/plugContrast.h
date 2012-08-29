// plugContrast.h : main header file for the plugContrast DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CplugContrastApp
// See plugContrast.cpp for the implementation of this class
//

class CplugContrastApp : public CWinApp
{
public:
	CplugContrastApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
