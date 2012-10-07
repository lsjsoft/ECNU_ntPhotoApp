#include "StdAfx.h"
#include "ntCommandMgr.h"
#include "ntCommand.h"

/// 最大记录历史
const unsigned int g_uiMaxCmdHistory = 30;

ntCommandMgr::ntCommandMgr(void)
{
	m_pRootCmd = ntNew ntCommand(this);
	m_pCurrentCmd = m_pRootCmd;
}

ntCommandMgr::~ntCommandMgr(void)
{
	destroyAll();
}

void ntCommandMgr::pushCmd( ntCommand* pCmd )
{
	ntCommand* pCurr = m_pCurrentCmd;
	ntCommand* pNext = m_pCurrentCmd->getNextCmd();
	destroyList(pNext);

	pCurr->setNextCmd(pCmd);
	pCmd->setPrevCmd(pCurr);
	m_pCurrentCmd = pCmd;

	unsigned int count= 0;
	ntCommand* p = m_pRootCmd;
	do
	{
		p = p->getNextCmd();
		++count;
	}
	while(p);

	while(count > g_uiMaxCmdHistory)
	{
		ntCommand* p = m_pRootCmd;
		m_pRootCmd = m_pRootCmd->getNextCmd();
		delete p;
		--count;
	}
}

void ntCommandMgr::destroyList( ntCommand* pCmd )
{
	while(pCmd)
	{
		ntCommand* pNext= pCmd->getNextCmd();
		delete pCmd;
		pCmd = pNext;
	}
}

void ntCommandMgr::redo()
{
	ntCommand* pNextCmd= m_pCurrentCmd->getNextCmd();
	if (pNextCmd)
	{
		pNextCmd->redo();
		m_pCurrentCmd = pNextCmd;
	}	
}

void ntCommandMgr::undo()
{
	ntCommand* pPrevCmd= m_pCurrentCmd->getPrevCmd();
	if (pPrevCmd)
	{
		m_pCurrentCmd->undo();
		m_pCurrentCmd = pPrevCmd;
	}
}

void ntCommandMgr::destroyAll()
{
	destroyList(m_pRootCmd);

	m_pRootCmd = 0;
	m_pCurrentCmd = 0;
}

bool ntCommandMgr::canRedo()
{
	return m_pCurrentCmd->getNextCmd() != NULL;
}

bool ntCommandMgr::canUndo()
{
	return m_pCurrentCmd != m_pRootCmd;
}
