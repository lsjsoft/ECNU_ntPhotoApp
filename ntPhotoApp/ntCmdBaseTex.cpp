#include "StdAfx.h"
#include "ntCmdBaseTex.h"
#include "ntService.h"
#include "ntTextureService.h"

ntCmdBaseTex::ntCmdBaseTex( ntCommandMgr* pMgr, ntTexture32Ptr handleTex )
: ntCommand(pMgr)
, m_handleTex(handleTex)
{
	m_orignalTex = handleTex->clone();
}

ntCmdBaseTex::~ntCmdBaseTex(void)
{
}

void ntCmdBaseTex::redo()
{
	m_handleTex->copyFrom( m_processTex );
}

void ntCmdBaseTex::undo()
{
	m_handleTex->copyFrom( m_orignalTex );
}

 
