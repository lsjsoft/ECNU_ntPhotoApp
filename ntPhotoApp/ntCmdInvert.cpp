#include "StdAfx.h"
#include "ntCmdInvert.h"
#include "ntPlugBaseService.h"
#include "ntService.h"
#include "ntTextureService.h"

ntCmdInvert::ntCmdInvert(ntCommandMgr* pMgr, ntTexture32Ptr tex)
: ntCmdBaseTex(pMgr, tex)
{
}

ntCmdInvert::~ntCmdInvert(void)
{
}

bool ntCmdInvert::excuteSimpleCmd( ntTexture32Ptr tex )
{
	ntGetService(ntPlugBaseService)->makeInvert(tex);
	return true;
}

