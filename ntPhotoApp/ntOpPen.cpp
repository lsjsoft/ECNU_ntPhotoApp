#include "StdAfx.h"
#include "ntOpPen.h"
#include "Resource.h"
#include "ntPhotoAppDoc.h"
#include "ntAppHelper.h"
#include "ntMainFrm.h"
#include "ntCmdAppendTex.h"
#pragma warning(disable: 4311)

extern BOOL wiz_3d_clip_2d_line(const ntRecti* vb_p, 
								ntPointi* io_begin_p, ntPointi* io_end_p);

static const std::string penName= "pen";

void drawPenLine(ntTexture32Ptr tex, const ntPointi* _pSrc, 
				 const ntPointi* _pDest, unsigned int dwColor)
{
	ntPointi a(*_pSrc), b(*_pDest);

	ntRecti m_rcCanvasRect(0, 0, tex->getWidth(), tex->getHeight());
	if (! wiz_3d_clip_2d_line(&m_rcCanvasRect, &a, &b))
	{
		return;
	}

	ntPointi* pSrc = &a;
	ntPointi* pDest = &b;

	int dx = pDest->x - pSrc->x;
	int dy = pDest->y - pSrc->y;

	if( dx == 0 && dy == 0 )
	{
		return;
	}

	unsigned int* pDestData= 0x00;

	if( ntAbs(dx) >= ntAbs(dy) )
	{
		int x_add = (dx > 0) ? 1 : -1;
		for(int x = 0; x != dx; x += x_add )
		{
			int y = x* dy / dx;
			tex->drawPixel(pSrc->x+x, pSrc->y+y, dwColor);
		}
	}
	else
	{
		int y_add = (dy > 0) ? 1 : -1;
		for(int y = 0; y != dy; y += y_add )
		{
			int x = y*dx/dy;
			tex->drawPixel(pSrc->x+x, pSrc->y+y, dwColor);
		}
	}
}


ntOpPen::ntOpPen(void)
: ntOp(ntOpPen::_GetOpId())
, m_hPen(0)
, m_bLeftPress(false)
{
	m_color.red= 0xFF;
	m_color.green = m_color.blue = 0x00;
	m_color.alpha = 0xFF;
}

ntOpPen::~ntOpPen(void)
{
}

void ntOpPen::onEnter()
{
	ntPhotoAppDoc* pDoc= ntGetActiveDoc();

	ntTexture32Ptr src= pDoc->getWorkTexture();
	if (src)
	{
		m_opTex= src->createCompatibleTex();
		pDoc->AddTex(penName, m_opTex);
	}	
}

void ntOpPen::onLeave()
{
	ntPhotoAppDoc* pDoc= ntGetActiveDoc();
	pDoc->RemoveTex(penName);
}

opIdType ntOpPen::_GetOpId()
{
	return penName;
}

void ntOpPen::onInit()
{
	m_hPen = LoadCursor(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDC_PLUG_PEN) );
}

HCURSOR ntOpPen::getCursor()
{
	return m_hPen;
}

bool ntOpPen::OnMouseLBDown( const ntPointi& p, ntCpsBv bv )
{
	if (!m_opTex)
	{
		return false;
	}

	m_lastPoint = p;
	m_bLeftPress= true;

	m_opTex->drawPixel(p.x, p.y, m_color);

	ntUpdateActiveView();

	return true;
}

bool ntOpPen::OnMouseLBUp( const ntPointi& p, ntCpsBv bv )
{
	if (!m_opTex)
	{
		return false;
	}

	if (!m_bLeftPress)
	{
		return false;
	}

	m_bLeftPress= false;

	ntPhotoAppDoc* pDoc= ntGetActiveDoc();
	
	ntTexture32Ptr workTex= pDoc->getWorkTexture();
	ntCmdAppendTex* pPen= new ntCmdAppendTex(pDoc->getCmdMgr(), workTex, m_opTex);
	if ( pPen->run() )
	{
		m_opTex->erase();
	}

	ntUpdateActiveView();

	return true;
}

bool ntOpPen::OnMouseMove( const ntPointi& p, ntCpsBv bv )
{
	if (!m_opTex)
	{
		return false;
	}

	if (!m_bLeftPress)
	{
		return false;
	}

	drawPenLine( m_opTex, &m_lastPoint, &p, m_color);

	m_lastPoint= p;

	ntUpdateActiveView();

	return true;
}

void ntOpPen::SetTrans( int iValue )
{
	m_color.alpha = iValue;
}

void ntOpPen::SetColor( COLORREF color )
{
	m_color.fromCOLORREF(color);
}
