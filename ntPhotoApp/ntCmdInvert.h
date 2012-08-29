#pragma once
#include "ntCommand.h"
#include "ntCmdBaseTex.h"

class ntCmdInvert: public ntCmdBaseTex
{
public:
	ntCmdInvert(ntCommandMgr* pMgr, ntTexture32Ptr tex);
	~ntCmdInvert(void);
	virtual bool excuteSimpleCmd(ntTexture32Ptr tex);
};
