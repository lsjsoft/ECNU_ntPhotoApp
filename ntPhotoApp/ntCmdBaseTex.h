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
	ntTexture32Ptr m_orignalTex;	/// 原始的
	ntTexture32Ptr m_processTex;	/// 处理后的
	ntTexture32Ptr m_handleTex;		/// 当前的
};
