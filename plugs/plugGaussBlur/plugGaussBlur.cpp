// plugGaussBlur.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "plugGaussBlur.h"
#include "plugSettingDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

//
//TODO: If this DLL is dynamically linked against the MFC DLLs,
//		any functions exported from this DLL which call into
//		MFC must have the AFX_MANAGE_STATE macro added at the
//		very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the 
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//


// CplugGaussBlurApp

BEGIN_MESSAGE_MAP(CplugGaussBlurApp, CWinApp)
END_MESSAGE_MAP()


static const ntPlugInfo g_PlugInfo=
{
	"{9E4DC356-260C-D781-18E1-FE7FCA5D22E6}",
	"高斯模糊",
	"可以对图像进行模糊处理",
	1,
};

PLUG_API bool plugGetInfo(ntPlugInfo* pInfo)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());
	if (!pInfo)
	{
		return false;
	}

	memcpy(pInfo, &g_PlugInfo, sizeof(ntPlugInfo));
	return true;
}

PLUG_API bool plugExcute(ntPlugCallParam* pParam)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	if (!pParam)
	{
		return false;
	}

	if (!pParam->checkValud())
	{
		return false;
	}

	bool bSucc = false;
	plugSettingDlg dlg(pParam);
	if (dlg.DoModal() == IDOK)
	{
		bSucc= true;
	}

	return bSucc;
}


// CplugGaussBlurApp construction

CplugGaussBlurApp::CplugGaussBlurApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CplugGaussBlurApp object

CplugGaussBlurApp theApp;


// CplugGaussBlurApp initialization

BOOL CplugGaussBlurApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
