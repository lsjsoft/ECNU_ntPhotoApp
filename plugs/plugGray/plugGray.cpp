// plugGray.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "plugGray.h"
#include "..\..\include\Plug\ntPlugInterface.h"

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


// CplugGrayApp

BEGIN_MESSAGE_MAP(CplugGrayApp, CWinApp)
END_MESSAGE_MAP()


// CplugGrayApp construction

CplugGrayApp::CplugGrayApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CplugGrayApp object

CplugGrayApp theApp;

BOOL CplugGrayApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}

CplugGrayApp::~CplugGrayApp()
{

}

static const ntPlugInfo g_PlugInfo=
{
	"{84FDF50E-B653-08A3-9BC1-D80255465C87}",
	"灰化",
	"可以把图像灰化",
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
		pDest->r = pDest->g = pDest->b = 
			( ( pSrc->r * 76 + pSrc->g * 150 + pSrc->b * 29 ) >> 8 );

		pDest->a = pSrc->a;

		++pSrc;
		++pDest;
	}

	return true;
}

