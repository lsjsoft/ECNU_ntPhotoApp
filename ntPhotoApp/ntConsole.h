#pragma once
#include "ntServiceBase.h"

class ntConsole: public ntServiceBase
{
public:
	ntImplService(ntConsole);
	ntConsole(void);
	~ntConsole(void);

	void open();

	void close();

	virtual void start();

	virtual void update(float fDeltaTime);

protected:
	HANDLE ms_hScreenBuffer;
	FILE* ms_pkStdoutFile;
	FILE* ms_pkStderrFile;
};
