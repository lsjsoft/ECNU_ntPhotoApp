// plugContrast.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "plugContrast.h"
#include "plugContrastMain.h"

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


// CplugContrastApp

BEGIN_MESSAGE_MAP(CplugContrastApp, CWinApp)
END_MESSAGE_MAP()


// CplugContrastApp construction

CplugContrastApp::CplugContrastApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CplugContrastApp object

CplugContrastApp theApp;


// CplugContrastApp initialization

BOOL CplugContrastApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

cntPlugImplInfo("{539C66AA-8A83-971F-9437-73630B2DEF3D}", "调节对比度",
	"可以对图像进行对比度调节", 1)


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

	bool bSucc= false;

	plugContrastMain dlg(pParam);
	if ( dlg.DoModal() == IDOK )
	{
		bSucc= true;
	}

	return bSucc;
}
