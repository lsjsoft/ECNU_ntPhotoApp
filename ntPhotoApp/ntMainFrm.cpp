// MainFrm.cpp : implementation of the ntMainFrame class
//

#include "stdafx.h"
#include "ntPhotoApp.h"

#include "ntMainFrm.h"
#include "ntHistogramView.h"
#include "ntPhotoAppView.h"
#include "ntPhotoAppDoc.h"
#include "ntPlugBaseService.h"
#include "ntService.h"
#include "ntPlugExtendService.h"


// ntMainFrame

IMPLEMENT_DYNAMIC(ntMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(ntMainFrame, CMDIFrameWnd)
	ON_WM_CREATE()
	ON_COMMAND(ID_VIEW_HISTOGRAM, &ntMainFrame::OnViewHistogram)
	ON_MESSAGE(WM_UPDATE_HISTOGRAM, &ntMainFrame::OnUpdateActiveHistogram)
	ON_COMMAND(ID_FILE_SAVE, &ntMainFrame::OnFileSave)
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};


// ntMainFrame construction/destruction

ntMainFrame::ntMainFrame()
: m_pHistogramView(0)
{
	// TODO: add member initialization code here
}

ntMainFrame::~ntMainFrame()
{
	delete m_pHistogramView;
}


int ntMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Delete these three lines if you don't want the toolbar to be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

	return 0;
}

BOOL ntMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return TRUE;
}


// ntMainFrame diagnostics

#ifdef _DEBUG
void ntMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void ntMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG


// ntMainFrame message handlers




void ntMainFrame::OnViewHistogram()
{
	// TODO: Add your command handler code here
	if (!m_pHistogramView)
	{
		m_pHistogramView= new ntHistogramView();
		m_pHistogramView->Create(IDD_HISTOGRAM);
		m_pHistogramView->ShowWindow(SW_SHOW);
		m_pHistogramView->updateHistogram();
		return;
	}
	
	BOOL bVisible= m_pHistogramView->IsWindowVisible();
	m_pHistogramView->ShowWindow( bVisible? SW_HIDE : SW_SHOW);
	m_pHistogramView->updateHistogram();
}

LRESULT ntMainFrame::OnUpdateActiveHistogram( WPARAM wParam, LPARAM lParam )
{
	if (m_pHistogramView)
	{
		m_pHistogramView->updateHistogram();
	}

	return 0;
}

void ntMainFrame::OnFileSave()
{
 	ntPhotoAppDoc* pDoc= dynamic_cast<ntPhotoAppDoc*>(
 		GetActiveDocument());
	if (!pDoc)
	{
		return;
	}

	pDoc->DoSave(NULL);
}

void ntMainFrame::GetMessageString( UINT nID, CString& rMessage ) const
{
	if (IsPlugMenuId(nID))
	{
		ntPlugExtendService* pSvr= ntGetService(ntPlugExtendService);
		const ntPlugInfo* pInfo = pSvr->getPlug(nID);
		if (pInfo)
		{
			rMessage = pInfo->desc;
			return;
		}
	}

	__super::GetMessageString(nID, rMessage);
}

