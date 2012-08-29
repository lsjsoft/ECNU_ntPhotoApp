#include "StdAfx.h"
#include "ntService.h"
#include "ntTimeService.h"
#include "ntConsole.h"
#include "ntTextureService.h"

ntService* g_pService = NULL;

ntService::ntService(void)
{
}

ntService::~ntService(void)
{
	uninitialize();
}

ntService* ntService::getIns()
{
	return g_pService;
}

void ntService::uninitialize()
{
	end();

	destroy();
}

void ntService::init()
{
	if (!g_pService)
	{
		g_pService = new ntService();
	}
}

void ntService::release()
{
	if (g_pService)
	{
		delete g_pService;
		g_pService = NULL;
	}
}

void ntService::update( float fDeltaTime )
{
	ntSvrMap::iterator it = m_svrMap.begin();
	while( it!= m_svrMap.end())
	{
		ntServiceBase* pSvr= it->second;
		pSvr->update(fDeltaTime);
		++it;		
	}
}

bool ntService::add( ntServiceBase* pSvr )
{
	if (!pSvr)
	{
		return false;
	}

	const char* szSvrName= pSvr->getServiceName();
	if (!szSvrName)
	{
		return false;
	}

	ntSvrMap::iterator it = m_svrMap.find(szSvrName);
	if ( it != m_svrMap.end() )
	{
		return false;
	}

	m_svrMap[std::string(szSvrName)]= pSvr;

	m_svrVector.push_back( pSvr );

	return true;
}

void ntService::start()
{
	for( ntSvrPtrVector::iterator it = m_svrVector.begin();
		it != m_svrVector.end(); ++it)
	{
		(*it)->start();
	}
}

void ntService::end()
{
	for( ntSvrPtrVector::iterator it = m_svrVector.begin();
		it != m_svrVector.end(); ++it)
	{
		(*it)->end();
	}
}

void ntService::destroy()
{
	for(ntSvrPtrVector::reverse_iterator it = m_svrVector.rbegin();
		it != m_svrVector.rend(); ++it)
	{
		ntServiceBase* pSvr= *it;
		delete pSvr;
	}

	m_svrVector.clear();
	m_svrMap.clear();
}
