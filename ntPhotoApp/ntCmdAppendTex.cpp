#include "StdAfx.h"
#include "ntCmdAppendTex.h"

ntCmdAppendTex::ntCmdAppendTex(ntCommandMgr* pMgr, ntTexture32Ptr handTex
				   , ntTexture32Ptr append)
: ntCmdBaseTex(pMgr, handTex)
, m_append(append)
{
}

ntCmdAppendTex::~ntCmdAppendTex(void)
{
}

bool ntCmdAppendTex::doCmd()
{
	if (m_append && m_handleTex)
	{
		m_append->bltTo( m_handleTex );
		m_processTex = m_handleTex->clone();
		return true;
	}

	return false;
}
