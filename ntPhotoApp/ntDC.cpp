#include "StdAfx.h"
#include "ntDC.h"

ntDC::ntDC(void)
: m_hDC(0)
{
}

ntDC::~ntDC(void)
{
	reset();
}

void ntDC::select( HGDIOBJ hObj )
{
	SelectObject(m_hDC, hObj);
}

void ntDC::reset()
{
	if (m_hDC) 
	{ 
		DeleteDC(m_hDC); 
		m_hDC = 0; 
	}
}

bool ntDC::create( HDC dc )
{
	reset();
	m_hDC = CreateCompatibleDC(dc);
	return m_hDC != 0;
}

