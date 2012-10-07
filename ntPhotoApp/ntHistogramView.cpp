// ntHistogramView.cpp : implementation file
//

#include "stdafx.h"
#include "ntPhotoApp.h"
#include "ntHistogramView.h"
#include "ntScreen.h"
#include "ntAppHelper.h"
#include <afxwin.h>
#include "ntPhotoAppDoc.h"
#include "ntTexture.h"
#include "ntPhotoAppView.h"
#include "ntPlugBaseService.h"
#include "ntService.h"
#include "ntMainFrm.h"
#include <math.h>


// ntHistogramView dialog
const DWORD UPDATE_HISTOGRAM_VIEW = 100;

IMPLEMENT_DYNAMIC(ntHistogramView, CDialog)

ntHistogramView::ntHistogramView(CWnd* pParent /*=NULL*/)
	: CDialog(ntHistogramView::IDD, pParent)
	, m_uiTextureVersion(ntInvalidId)
	, m_uiTextureId(ntInvalidId)
{
}

ntHistogramView::~ntHistogramView()
{
}

void ntHistogramView::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HISTOGRAM_CANVAS, m_histogramCanvas);
}


BEGIN_MESSAGE_MAP(ntHistogramView, CDialog)
//	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_WM_TIMER()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// ntHistogramView message handlers

BOOL ntHistogramView::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	m_histogramCanvas.initClt();

	SetTimer( UPDATE_HISTOGRAM_VIEW, 1000, NULL);

	ntWindowRect rect;
	m_histogramCanvas.GetWindowRect(&rect);
	ScreenToClient(&rect);
	rect.setWidthByLeft(256);
	rect.setHeightByTop(128);

	::MoveWindow(m_histogramCanvas.GetSafeHwnd(),
		rect.left, rect.top, rect.width(), rect.height(), TRUE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


void ntHistogramView::OnPaint()
{
	CPaintDC dc(this);

	m_histogramCanvas.beginDraw();

	if (m_spHistogram)
	{
		ntScreen* pScreen = m_histogramCanvas.getScreen();

		ntPixel32* pPixel= (ntPixel32*)pScreen->getBackbackBuffer();
		const ntRecti& canvasRect= pScreen->getCanvasRect();
		unsigned int uiMax= ( m_spHistogram->getMax() );
		float fRate= ( canvasRect.h / (float)uiMax );

		ntBufferPtr buff= ntNew ntBuffer();
		buff->allocBuffer<unsigned int>(canvasRect.w);

		unsigned int* pIntBuff= buff->getBuffer<unsigned int>();
		m_spHistogram->full( m_spHistogram->rgb, pIntBuff, canvasRect.w);

		for(int i=0; i<canvasRect.w; ++i)
		{
			float fLength = (float)pIntBuff[i];
			fLength *= fRate;
			fLength = canvasRect.h - fLength;
			ntPointi p0(i, (int)fLength), p1(i, canvasRect.h);
			pScreen->drawLine(&p0, &p1, 0xFFFFFFFF);
		}
	}

	m_histogramCanvas.endDraw();

}

void ntHistogramView::updateHistogram()
{
	CFrameWnd* pChildFrame= ntGetMainFrame()->GetActiveFrame();
	if (!pChildFrame)
	{
		return;
	}

	ntPhotoAppView* pAppView= static_cast<ntPhotoAppView*>(
		pChildFrame->GetActiveView());

	if (! (pAppView && pAppView->GetDocument()))
	{
		if (m_spHistogram)
		{
			makeInvalid();
		}

		return;
	}

	ntTexture32Ptr workText= pAppView->GetDocument()->getWorkTexture();
	if (! workText)
	{
		if (m_spHistogram)
		{
			makeInvalid();
		}

		return;
	}

	if (workText->equalx( m_uiTextureId, m_uiTextureVersion))
	{
		return;
	}

	ntPlugBaseService* pCommPlug= ntGetService(ntPlugBaseService);
	m_spHistogram = pCommPlug->getHistogram(workText);

	m_uiTextureVersion = workText->getVersion();
	m_uiTextureId = workText->getId();

	Invalidate();
}

void ntHistogramView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if( UPDATE_HISTOGRAM_VIEW == nIDEvent )
	{
		updateHistogram();
	}

	CDialog::OnTimer(nIDEvent);
}

void ntHistogramView::makeInvalid()
{
	m_spHistogram = 0;
	m_uiTextureId = ntInvalidId;
	m_uiTextureVersion = ntInvalidId;
	Invalidate();
}

void ntHistogramView::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
}

LRESULT ntHistogramView::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CDialog::WindowProc(message, wParam, lParam);
}
