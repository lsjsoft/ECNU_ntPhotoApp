#include "StdAfx.h"
#include "ntCommand.h"
#include "ntCommandMgr.h"

ntCommand::ntCommand(ntCommandMgr* pMgr)
: m_pNextCmd(0)
, m_pPrevCmd(0)
, m_pMgr(pMgr)
{
}

ntCommand::~ntCommand(void)
{
}

bool ntCommand::run()
{
	if (doCmd())
	{
		m_pMgr->pushCmd(this);
		return true;
	}

	return false;
}
