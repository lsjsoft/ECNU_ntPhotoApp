#pragma once
#include "ntTexture.h"
#include "ntCommand.h"

class ntCommandMgr;

class ntCmdBaseTex: public ntCommand
{
public:
	ntCmdBaseTex(ntCommandMgr* pMgr, ntTexture32Ptr handTex);
	~ntCmdBaseTex(void);

protected:
	virtual void redo();
	virtual void undo();

protected:
	ntTexture32Ptr m_orignalTex;
	ntTexture32Ptr m_processTex;
	ntTexture32Ptr m_handleTex;
};
