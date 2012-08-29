// ntPhotoAppView.cpp : implementation of the ntPhotoAppView class
//

#include "stdafx.h"
#include "ntPhotoApp.h"

#include "ntPhotoAppDoc.h"
#include "ntPhotoAppView.h"
#include "ntScreen.h"
#include "ntTextureService.h"
#include "ntService.h"
#include "ntAssetService.h"
#include "ntCommandMgr.h"
#include "ntMainFrm.h"
#include "ntAppHelper.h"
#include "ntPlugExtendService.h"

// ntPhotoAppView

IMPLEMENT_DYNCREATE(ntPhotoAppView, CView)

BEGIN_MESSAGE_MAP(ntPhotoAppView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_SIZE()
	ON_COMMAND(ID_EDIT_UNDO, &ntPhotoAppView::OnEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, &ntPhotoAppView::OnUpdateEditUndo)
	ON_COMMAND(ID_EDIT_REDO, &ntPhotoAppView::OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, &ntPhotoAppView::OnUpdateEditRedo)

END_MESSAGE_MAP()

// ntPhotoAppView construction/destruction

ntPhotoAppView::ntPhotoAppView()
: m_pScreen(0)
{
	// TODO: add construction code here

}

ntPhotoAppView::~ntPhotoAppView()
{
	delete m_pScreen;
}

BOOL ntPhotoAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// ntPhotoAppView drawing

void ntPhotoAppView::OnDraw(CDC* /*pDC*/)
{
	renderView();
}


// ntPhotoAppView printing

BOOL ntPhotoAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void ntPhotoAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void ntPhotoAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}


// ntPhotoAppView diagnostics

#ifdef _DEBUG
void ntPhotoAppView::AssertValid() const
{
	CView::AssertValid();
}

void ntPhotoAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

ntPhotoAppDoc* ntPhotoAppView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(ntPhotoAppDoc)));
	return (ntPhotoAppDoc*)m_pDocument;
}

#endif //_DEBUG


// ntPhotoAppView message handlers

void ntPhotoAppView::OnInitialUpdate()
{
	if (!m_pScreen)
	{
		ntLogFunction();

		m_pScreen = ntNew ntScreen;
		m_pScreen->setPresentHandle( GetSafeHwnd() );
		RECT rect;
		GetClientRect(&rect);
		int cx= rect.right - rect.left;
		int cy= rect.bottom - rect.top;
		m_pScreen->create(cx, cy);
		m_background = ntGetService(ntAssetService)->getBackground();

		ntPhotoAppDoc* pDoc = GetDocument();
		const CString& str= pDoc->GetPathName();
		std::string Title= (LPCSTR)(str);
		CString pathName= ntGetShortName( Title ).c_str();
		SetWindowText(pathName);
	}



}

void ntPhotoAppView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	if (m_pScreen)
	{
		m_pScreen->resize(cx, cy);
	}
}

void ntPhotoAppView::renderView()
{
	ntPhotoAppDoc* pDoc = GetDocument();
	if (!pDoc)
	{
		return;
	}

	m_pScreen->beginRender();

	m_pScreen->drawTextureWrap( m_background, noClipRecti);

	ntTexture32Ptr spWorkTexture= pDoc->getWorkTexture();
	if (spWorkTexture)
	{
		m_pScreen->drawTextureEx( spWorkTexture, noClipRecti);

		ntRecti drawRect(0, 0, spWorkTexture->getWidth(),
			spWorkTexture->getHeight());

		m_pScreen->drawRect( drawRect, ntColorBlack);
	}

	m_pScreen->endRender();	
}

void ntPhotoAppView::OnEditUndo()
{
	// TODO: Add your command handler code here
	ntPhotoAppDoc* pDoc = GetDocument();
	pDoc->getCmdMgr()->undo();
	Invalidate(TRUE);
}

void ntPhotoAppView::OnUpdateEditUndo(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	ntPhotoAppDoc* pDoc = GetDocument();
	pCmdUI->Enable( pDoc->getCmdMgr()->canUndo() );
}

void ntPhotoAppView::OnEditRedo()
{
	ntPhotoAppDoc* pDoc = GetDocument();
	pDoc->getCmdMgr()->redo();
	Invalidate(TRUE);
}

void ntPhotoAppView::OnUpdateEditRedo(CCmdUI *pCmdUI)
{
	ntPhotoAppDoc* pDoc = GetDocument();
	pCmdUI->Enable( pDoc->getCmdMgr()->canRedo() );
}

void ntPhotoAppView::OnActivateView(BOOL bActivate, CView* pActivateView, 
									CView* pDeactiveView)
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);

	ntGetMainFrame()->SendMessage(WM_UPDATE_HISTOGRAM, 
		(WPARAM)pActivateView, (LPARAM)bActivate);

}

void ntPhotoAppView::OnUpdatePlugRange( CCmdUI *pCmdU )
{
	ntPlugExtendService* pSvr= ntGetService(ntPlugExtendService);
	const ntPlugInfo* pInfo= pSvr->getPlug(pCmdU->m_nID);
	pCmdU->Enable(true);
}
