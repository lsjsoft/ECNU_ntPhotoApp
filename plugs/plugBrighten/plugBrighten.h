// plugBrighten.h : main header file for the plugBrighten DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CplugBrightenApp
// See plugBrighten.cpp for the implementation of this class
//

class CplugBrightenApp : public CWinApp
{
public:
	CplugBrightenApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
