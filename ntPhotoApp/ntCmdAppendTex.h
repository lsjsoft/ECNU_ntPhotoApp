#pragma once
#include "ntCmdBaseTex.h"

class ntCmdAppendTex: public ntCmdBaseTex
{
public:
	ntCmdAppendTex(ntCommandMgr* pMgr, ntTexture32Ptr handTex, ntTexture32Ptr append);
	~ntCmdAppendTex(void);

	virtual bool doCmd();

protected:
	ntTexture32Ptr m_append;
};
