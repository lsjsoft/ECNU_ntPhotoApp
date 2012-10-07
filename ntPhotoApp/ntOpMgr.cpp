#include "StdAfx.h"
#include "ntOpMgr.h"
#include "ntMsgHandle.h"
#include "ntOp.h"
#include "ntOpPen.h"
#include "ntOpNone.h"
#include "ntOpCircle.h"

ntOpMgr::ntOpMgr(void)
: m_pActive(0)
, m_hDefault(0)
{
}

ntOpMgr::~ntOpMgr(void)
{
}

bool ntOpMgr::AddOp( ntOp* pOp )
{
	opIdType id= pOp->GetOpId();
	if ( findOp(id) )
	{
		return false;
	}

	pOp->onInit();

	m_Ops.insert( std::make_pair( id, pOp) );
	return true;
}

bool ntOpMgr::Active(const opIdType& opId)
{
	ntOp* pOp = findOp(opId);
	if (pOp)
	{
		if (m_pActive)
		{
			m_pActive->onLeave();
		}

		m_pActive = pOp;

		if (m_pActive)
		{
			m_pActive->onEnter();
		}

		return true;
	}

	return false;
}

bool ntOpMgr::OnMouseLBDown( const ntPointi& p, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnMouseLBDown(p, bv);
	}
	return false;
}

bool ntOpMgr::OnMouseLBUp( const ntPointi& p, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnMouseLBUp(p, bv);
	}
	return false;
}

bool ntOpMgr::OnMouseMove( const ntPointi& p, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnMouseMove(p, bv);
	}
	return false;
}

bool ntOpMgr::OnMouseRBDown( const ntPointi& p, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnMouseRBDown(p, bv);
	}
	return false;
}

bool ntOpMgr::OnMouseRUp( const ntPointi& p, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnMouseRUp(p, bv);
	}
	return false;
}

bool ntOpMgr::OnKeyDown( char key, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnKeyDown(key, bv);
	}
	return false;
}

ntOp* ntOpMgr::findOp( const opIdType& opId )
{
	ntOps::iterator it= m_Ops.find( opId );
	if (it == m_Ops.end())
	{
		return NULL;
	}

	return it->second;
}

void ntOpMgr::destroyAll()
{
	for each(const std::pair<opIdType, ntOp*>& p in m_Ops)
	{
		p.second->onRelease();

		delete p.second;
	}
	m_Ops.clear();
}

bool ntOpMgr::OnKeyUp( char key, ntCpsBv bv )
{
	if (m_pActive)
	{
		return m_pActive->OnKeyUp(key, bv);
	}
	return false;
}

void ntOpMgr::start()
{
	m_hDefault = ::LoadCursor(NULL, IDC_ARROW);

	AddOp( new ntOpPen());
	AddOp( new ntOpNone());
	AddOp( new ntOpCircle());

	Active<ntOpNone>();
}

void ntOpMgr::end()
{
	destroyAll();
}

HCURSOR ntOpMgr::getActiveCursor()
{
	HCURSOR hCursor= 0;

	if (m_pActive)
	{
		hCursor= m_pActive->getCursor();
	}

	if (!hCursor)
	{
		hCursor = m_hDefault;
	}

	return hCursor;
}
