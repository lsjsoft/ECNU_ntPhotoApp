// plugInvert.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "plugInvert.h"

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


// CplugInvertApp

BEGIN_MESSAGE_MAP(CplugInvertApp, CWinApp)
END_MESSAGE_MAP()


// CplugInvertApp construction

CplugInvertApp::CplugInvertApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CplugInvertApp object

CplugInvertApp theApp;


// CplugInvertApp initialization

BOOL CplugInvertApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

static const ntPlugInfo g_PlugInfo=
{
	"{343FBE11-4A03-4660-FB54-39D818EE9C57}",
	"反向",
	"可以把图像反向",
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

	unsigned int w = pParam->pSource->m_uiWidth;
	unsigned int h = pParam->pSource->m_uiHeight;
	if (! pParam->pAllFunc(pParam->pOutput, w, h))
	{
		return false;
	}

	ntPlugPix* pSrc = pParam->pSource->m_pPixelData;
	ntPlugPix* pDest= (*pParam->pOutput)->m_pPixelData;

	for( unsigned int i=0; i< w * h; ++i)
	{
		pDest->r = 0xFF - pSrc->r;
		pDest->g = 0xFF - pSrc->g;
		pDest->b = 0xFF - pSrc->b;
		pDest->a = pSrc->a;
		++pSrc, ++pDest;
	}

	return true;
}
