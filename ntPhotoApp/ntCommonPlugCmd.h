#pragma once
#include "ntCmdBaseTex.h"

class ntCommonPlugCmd: public ntCmdBaseTex
{
public:
	ntCommonPlugCmd(ntCommandMgr* pMgr, ntTexture32Ptr tex, const std::string& rkCmdName);
	~ntCommonPlugCmd(void);

	virtual bool doCmd();

	static void doCmd(ntCommandMgr* pMgr, ntTexture32Ptr tex, const std::string& rkCmdName);

protected:
	std::string m_kCmdName;
};
