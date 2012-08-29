// plugInvert.h : main header file for the plugInvert DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CplugInvertApp
// See plugInvert.cpp for the implementation of this class
//

class CplugInvertApp : public CWinApp
{
public:
	CplugInvertApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
