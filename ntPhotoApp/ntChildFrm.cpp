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
#include "ntOpNone.h"
#include "ntOpPen.h"
#include "ntOpMgr.h"
#include "ntDlgSelTrans.h"
#include "ntOpCircle.h"

#define TOOLBAR_STYLE (WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC)
// ntChildFrame

IMPLEMENT_DYNCREATE(ntChildFrame, CMDIChildWnd)

BEGIN_MESSAGE_MAP(ntChildFrame, CMDIChildWnd)
	ON_WM_CREATE()
	ON_WM_MDIACTIVATE()
	ON_COMMAND_RANGE(WM_PLUG_MENU_BASE_ID, WM_PLUG_MENU_MAX_ID, OnPlugMenuClick)
	ON_COMMAND(ID_MODE_NORMALMODE, &ntChildFrame::OnModeNormalmode)
	ON_COMMAND(ID_MODE_PENMODE, &ntChildFrame::OnModePenmode)
	ON_COMMAND(ID_SEL_PEN_COLOR, &ntChildFrame::OnSelPenColor)
	ON_COMMAND(ID_SEL_PEN_TRANS, &ntChildFrame::OnSelPenTrans)
	ON_COMMAND(ID_SWITCH_TO_NONE, &ntChildFrame::OnModeNormalmode)
	ON_COMMAND(ID_MODE_CIRCLE, &ntChildFrame::OnSelCircleMode)
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

	m_pPenToolbar= new CToolBar();
	if ( ! m_pPenToolbar->CreateEx(this, TBSTYLE_FLAT, TOOLBAR_STYLE) 
		|| !m_pPenToolbar->LoadToolBar(IDR_TOOLBAR_PEN))
	{
		delete m_pPenToolbar;
		m_pPenToolbar= NULL;
	}

	ShowControlBar(m_pPenToolbar, FALSE, FALSE);

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

void ntChildFrame::OnModeNormalmode()
{
	// TODO: Add your command handler code here
	ntPhotoAppView* pView= (ntPhotoAppView*)GetActiveView();
	pView->getOpMgr()->Active<ntOpNone>();
	
	HideToobar();
}

void ntChildFrame::OnModePenmode()
{
	ntPhotoAppView* pView= (ntPhotoAppView*)GetActiveView();
	pView->getOpMgr()->Active<ntOpPen>();

	HideToobar();

	if (m_pPenToolbar)
	{
		ShowControlBar(m_pPenToolbar, TRUE, FALSE);
	}
}

void ntChildFrame::OnSelPenColor()
{
	CColorDialog dlg;
	if ( IDOK != dlg.DoModal())
	{
		return;
	}

	COLORREF color= dlg.GetColor();
	
	ntPhotoAppView* pView= (ntPhotoAppView*)GetActiveView();
	ntOpMgr* pMgr= pView->getOpMgr();
	ntOp* pActiveOp= pMgr->getActiveOp();

	ntOpPen* pPen= pView->getOpMgr()->GetOp<ntOpPen>();
	ntOpCircle* pCircle= pView->getOpMgr()->GetOp<ntOpCircle>();
	if (pPen && pPen == pActiveOp)
	{
		pPen->SetColor(color);
	}
	else if (pCircle && pCircle == pActiveOp)
	{
		pCircle->SetColor(color);
	}
}

void ntChildFrame::OnSelPenTrans()
{
	ntDlgSelTrans dlg;
	if ( IDOK != dlg.DoModal())
	{
		return;
	}

	int color = dlg.m_iValue * 255 / 100;

	ntPhotoAppView* pView= (ntPhotoAppView*)GetActiveView();
	ntOpMgr* pMgr= pView->getOpMgr();
	ntOp* pActiveOp= pMgr->getActiveOp();

	ntOpPen* pPen= pView->getOpMgr()->GetOp<ntOpPen>();
	ntOpCircle* pCircle= pView->getOpMgr()->GetOp<ntOpCircle>();
	if (pPen && pPen == pActiveOp)
	{
		pPen->SetTrans(color);
	}
	else if (pCircle && pCircle == pActiveOp)
	{
		pCircle->SetTrans(color);
	}
}

void ntChildFrame::OnSelCircleMode()
{
	ntPhotoAppView* pView= (ntPhotoAppView*)GetActiveView();
	pView->getOpMgr()->Active<ntOpCircle>();

	HideToobar();

	if (m_pPenToolbar)
	{
		ShowControlBar(m_pPenToolbar, TRUE, FALSE);
	}
}

void ntChildFrame::HideToobar()
{
	if (m_pPenToolbar)
	{
		ShowControlBar(m_pPenToolbar, FALSE, FALSE);
	}
}
