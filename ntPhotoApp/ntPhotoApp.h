// ntPhotoApp.h : main header file for the ntPhotoApp application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// ntPhotoAppApp:
// See ntPhotoApp.cpp for the implementation of this class
//

class ntPhotoAppApp : public CWinApp
{
public:
	ntPhotoAppApp();
	~ntPhotoAppApp();

// Overrides
public:
	virtual BOOL InitInstance();
	BOOL OnIdle( LONG lCount );

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern ntPhotoAppApp theApp;