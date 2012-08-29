// ChildFrm.cpp : implementation of the ntChildFrame class
//
// ChildFrm.cpp : implementation of the ntChildFrame class
 
#include "stdafx.h"
#include "ntPhotoApp.h"

#include "ntChildFrm.h"
#include "ntPlugExtendService.h"
#include "ntService.h"
#include "ntPhotoAppView.h"
#include "ntCommonPlugCmd.h"
#include "ntPhotoAppDoc.h"


// ntChildFrame

IMPLEMENT_DYNCREATE(ntChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(ntChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_COMMAND_RANGE(WM_PLUG_MENU_BASE_ID, WM_PLUG_MENU_MAX_ID, OnPlugMenuClick)
END_MESSAGE_MAP()


// ntChildFrame construction/destruction

ntChildFrame::ntChildFrame()
{
	// TODO: add member initialization code here
}

ntChildFrame::~ntChildFrame()
{
}


BOOL ntChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}


// ntChildFrame diagnostics

#ifdef _DEBUG
void ntChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void ntChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG


// ntChildFrame message handlers

int ntChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

void ntChildFrame::OnMDIActivate( BOOL bActivate, CWnd* pActivateWnd, CWnd* pDeactivateWnd )
{
	__super::OnMDIActivate( bActivate, pActivateWnd, pDeactivateWnd);
}

void ntChildFrame::OnPlugMenuClick(UINT uId)
{
	ntPlugExtendService* pSvr= ntGetService(ntPlugExtendService);
	const ntPlugInfo* pPlugInfo= pSvr->getPlug(uId);
	if (!pPlugInfo)
	{
		return;
	}

	ntPhotoAppView* pView= (ntPhotoAppView*)GetActiveView();
	ntPhotoAppDoc* pDoc= pView->GetDocument();

	if (pDoc->getWorkTexture())
	{
		ntCommonPlugCmd::doCmd(pDoc->getCmdMgr(),
			pDoc->getWorkTexture(), pPlugInfo->guid);

		pDoc->UpdateAllViews(NULL);
	}
}
