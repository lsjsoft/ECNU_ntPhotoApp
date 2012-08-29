#include "StdAfx.h"
#include "ntCanvasCtl.h"
#include "ntScreen.h"

BEGIN_MESSAGE_MAP(ntCanvasCtl, CStatic)
	// Standard printing commands
	ON_WM_SIZE()
	//	ON_WM_ACTIVATEAPP()
END_MESSAGE_MAP()

ntCanvasCtl::ntCanvasCtl(void)
: m_pScreen(0)
{
}

ntCanvasCtl::~ntCanvasCtl(void)
{
	delete m_pScreen;
}

void ntCanvasCtl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
}

void ntCanvasCtl::initClt()
{
	m_pScreen = ntNew ntScreen;
	m_pScreen->createEx( GetSafeHwnd() );
	m_pScreen->setBackgroundColor( 0x00);
}

void ntCanvasCtl::beginDraw()
{
	m_pScreen->beginRender();
}

void ntCanvasCtl::endDraw()
{
	m_pScreen->endRender();
}

void ntCanvasCtl::OnSize( UINT nType, int cx, int cy )
{
	m_pScreen->resize(cx, cy);
}
