// plugGaussBlur.h : main header file for the plugGaussBlur DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CplugGaussBlurApp
// See plugGaussBlur.cpp for the implementation of this class
//

class CplugGaussBlurApp : public CWinApp
{
public:
	CplugGaussBlurApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
