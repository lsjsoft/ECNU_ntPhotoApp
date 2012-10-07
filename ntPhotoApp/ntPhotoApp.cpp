// ntPhotoApp.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ntPhotoApp.h"
#include "ntMainFrm.h"

#include "ntChildFrm.h"
#include "ntPhotoAppDoc.h"
#include "ntPhotoAppView.h"
#include "ntConsole.h"
#include "ntTextureService.h"
#include "ntTimeService.h"
#include "ntService.h"
#include "ntAssetService.h"
#include "ntPlugBaseService.h"
#include "ntPlugExtendService.h"
#include "ntMenuHelper.h"
#include "ntOpMgr.h"


// ntPhotoAppApp

BEGIN_MESSAGE_MAP(ntPhotoAppApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &ntPhotoAppApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// ntPhotoAppApp construction

ntPhotoAppApp::ntPhotoAppApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only ntPhotoAppApp object

ntPhotoAppApp theApp;


// ntPhotoAppApp initialization

BOOL ntPhotoAppApp::InitInstance()
{
#ifdef _DEBUG
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
#endif

	ntService::init();

  	ntLoadService(ntConsole);
  	ntLoadService(ntTextureService);
  	ntLoadService(ntTimeService);
  	ntLoadService(ntAssetService);
  	ntLoadService(ntPlugBaseService);
 	ntLoadService(ntPlugExtendService);

	ntService::getIns()->start();

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("Local AppWizard-Generated Applications"));
	LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CMultiDocTemplate* pDocTemplate;
	pDocTemplate = new CMultiDocTemplate(IDR_ntPhotoAppTYPE,
		RUNTIME_CLASS(ntPhotoAppDoc),
		RUNTIME_CLASS(ntChildFrame), // custom MDI child frame
		RUNTIME_CLASS(ntPhotoAppView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// create main MDI Frame window
	ntMainFrame* pMainFrame = new ntMainFrame;
	if (!pMainFrame || !pMainFrame->LoadFrame(IDR_MAINFRAME))
	{
		delete pMainFrame;
		return FALSE;
	}
	m_pMainWnd = pMainFrame;
	// call DragAcceptFiles only if there's a suffix
	//  In an MDI app, this should occur immediately after setting m_pMainWnd


	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing;
	ParseCommandLine(cmdInfo);


	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The main window has been initialized, so show and update it
	pMainFrame->ShowWindow(m_nCmdShow);
	pMainFrame->UpdateWindow();

	CMenu* pRootMenu = CMenu::FromHandle( pDocTemplate->m_hMenuShared );
	unsigned int uiCount= pRootMenu->GetMenuItemCount();
	CMenu* pPlugMenu= pRootMenu->GetSubMenu(uiCount-1);

	ntPlugExtendService* pExtendPlugSvr= ntGetService(ntPlugExtendService);
 	pExtendPlugSvr->appendToMenu(pPlugMenu);

	return TRUE;
}



// CAboutDlg dialog used for App About


// App command to run the dialog
void ntPhotoAppApp::OnAppAbout()
{
}

ntPhotoAppApp::~ntPhotoAppApp()
{
	ntServiceShutdown();
}

// ntPhotoAppApp message handlers
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

BOOL ntPhotoAppApp::OnIdle( LONG lCount )
{
	static DWORD dwTime= 0;
	if (dwTime == 0)
	{
		dwTime = timeGetTime();
		return TRUE;
	}

	DWORD currTime= timeGetTime();
	float fDelatTime= ( currTime - dwTime ) * 0.001f;
	dwTime = currTime;
	ntService::getIns()->update(fDelatTime);
	Sleep(100);
	return TRUE;
}

