#pragma once
#include "ntServiceBase.h"
#include "../include/Plug/ntPlugInterface.h"

class ntPlugExtendService: public ntServiceBase
{
	ntImplService(ntPlugExtendService);

public:
	ntPlugExtendService(void);
	~ntPlugExtendService(void);

	bool callPlug(const char* szGuid, ntPlugPixData* pSource, 
		ntPlugPixData** pOutput);

	const ntPlugInfo* getPlug(unsigned int uiId);

	void appendToMenu(CMenu* pRootMenu);

protected:

	pntPlugExcute findPlug(const std::string& rkGuid);

	virtual void start();

	virtual void end();

	void loadDll(const std::string& rkPathName);

	struct DllInfo
	{
		HMODULE m_Handle;
		pntPlugExcute m_pExcute;
	};

	typedef std::map< std::string, DllInfo > DllMap;
	DllMap m_FuncMaps;

	std::vector<ntPlugInfo> m_vPlugs;
};
