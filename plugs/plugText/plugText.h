// plugText.h : main header file for the plugText DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CplugTextApp
// See plugText.cpp for the implementation of this class
//

class CplugTextApp : public CWinApp
{
public:
	CplugTextApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
