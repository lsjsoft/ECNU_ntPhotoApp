// plugGray.h : main header file for the plugGray DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CplugGrayApp
// See plugGray.cpp for the implementation of this class
//

class CplugGrayApp : public CWinApp
{
public:
	CplugGrayApp();
	~CplugGrayApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
