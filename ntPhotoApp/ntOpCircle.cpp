#include "StdAfx.h"
#include "ntOpCircle.h"
#include "Resource.h"
#include "ntPhotoAppDoc.h"
#include "ntAppHelper.h"
#include "ntMainFrm.h"
#include "ntCmdAppendTex.h"
#pragma warning(disable: 4311)

static const std::string modeName= "circle";

inline void DrawCircleMirror(ntTexture32Ptr dst, int cx, int cy, int x, 
							 int y, const ntPixel32& color)
{
	dst->drawPixel(cx-x, cy-y, color);
	dst->drawPixel(cx-x, cy+y, color);
	dst->drawPixel(cx+x, cy-y, color);
	dst->drawPixel(cx+x, cy+y, color);

	dst->drawPixel(cx-y, cy-x, color);
	dst->drawPixel(cx-y, cy+x, color);
	dst->drawPixel(cx+y, cy-x, color);
	dst->drawPixel(cx+y, cy+x, color);
}

inline void DrawCircle(ntTexture32Ptr dst, int cx, int cy, unsigned int r, const ntPixel32& color)
{
	if (!dst)
	{
		return;
	}

	int x = 0, y = r;
	float d = 1.25f - r;

	while(x<=y)
	{
		DrawCircleMirror(dst, cx, cy, x, y, color);

		if (d<0)
		{
			d+= 2*x + 3;
		}
		else
		{
			d+= 2*x-2*y+5;
			y--;
		}
		x++;
	}
}

ntOpCircle::ntOpCircle(void)
: ntOp(ntOpCircle::_GetOpId())
, m_hCursor(0)
, m_bLeftPress(false)
{
	m_color.red= 0xFF;
	m_color.green = m_color.blue = 0x00;
	m_color.alpha = 0xFF;
}

ntOpCircle::~ntOpCircle(void)
{
}

void ntOpCircle::onEnter()
{
	ntPhotoAppDoc* pDoc= ntGetActiveDoc();

	ntTexture32Ptr src= pDoc->getWorkTexture();
	if (src)
	{
		m_opTex= src->createCompatibleTex();
		pDoc->AddTex(modeName, m_opTex);
	}	
}

void ntOpCircle::onLeave()
{
	ntPhotoAppDoc* pDoc= ntGetActiveDoc();
	pDoc->RemoveTex(modeName);
}

opIdType ntOpCircle::_GetOpId()
{
	return modeName;
}

void ntOpCircle::onInit()
{
	m_hCursor = LoadCursor(AfxGetInstanceHandle(), 
		MAKEINTRESOURCE(IDC_PLUG_CIRCLE) );
}

HCURSOR ntOpCircle::getCursor()
{
	return m_hCursor;
}

bool ntOpCircle::OnMouseLBDown( const ntPointi& p, ntCpsBv bv )
{
	if (!m_opTex)
	{
		return false;
	}

	m_centerPos = p;
	m_bLeftPress= true;

	return true;
}

bool ntOpCircle::OnMouseLBUp( const ntPointi& p, ntCpsBv bv )
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

bool ntOpCircle::OnMouseMove( const ntPointi& p, ntCpsBv bv )
{
	if (!m_opTex)
	{
		return false;
	}

	if (!m_bLeftPress)
	{
		return false;
	}

	m_opTex->erase();
	int r = m_centerPos.length(p);
	DrawCircle( m_opTex, m_centerPos.x, m_centerPos.y, r, m_color);

	ntUpdateActiveView();

	return true;
}

void ntOpCircle::SetTrans( int iValue )
{
	m_color.alpha = iValue;
}

void ntOpCircle::SetColor( COLORREF color )
{
	m_color.fromCOLORREF(color);
}
