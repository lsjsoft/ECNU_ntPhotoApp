#include "StdAfx.h"
#include "ntCommonPlugCmd.h"
#include "..\include\Plug\ntPlugInterface.h"
#include "ntTexture.h"
#include "ntService.h"
#include "ntPlugExtendService.h"

ntCommonPlugCmd::ntCommonPlugCmd(ntCommandMgr* pMgr, 
								 ntTexture32Ptr tex, 
								 const std::string& rkCmdName )
: ntCmdBaseTex(pMgr, tex)
, m_kCmdName(rkCmdName)
{

}

ntCommonPlugCmd::~ntCommonPlugCmd(void)
{
}

bool ntCommonPlugCmd::doCmd()
{
	ntPlugPixData d;
	d.m_pPixelData = (ntPlugPix*)m_handleTex->getBuffer();
	d.m_uiWidth = m_handleTex->getWidth();
	d.m_uiHeight = m_handleTex->getHeight();

	ntPlugExtendService* pService= ntGetService(ntPlugExtendService);
	ntPlugPixData* pOutput= NULL;

	bool bSucc= false;

	if ( pService->callPlug(m_kCmdName.c_str(), &d, &pOutput) )
	{
		m_handleTex->accept((ntPixel32*)pOutput->m_pPixelData, pOutput->m_uiWidth, 
			pOutput->m_uiHeight);

		m_processTex = m_handleTex->clone();

		bSucc= true;
	}

	delete pOutput;

	return bSucc;
}

void ntCommonPlugCmd::doCmd(ntCommandMgr* pMgr, ntTexture32Ptr tex, 
							const std::string& rkCmdName )
{
	ntCommonPlugCmd* pCmd= new ntCommonPlugCmd(pMgr, tex, rkCmdName);
	pCmd->run();
}
