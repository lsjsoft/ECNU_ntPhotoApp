#include "StdAfx.h"
#include "ntConsole.h"

ntConsole::ntConsole(void)
: ms_pkStderrFile(0)
, ms_hScreenBuffer(0)
, ms_pkStdoutFile(0)
{

}

ntConsole::~ntConsole(void)
{
	close();
}

void ntConsole::close()
{
	if (ms_pkStderrFile)
	{
		fclose(ms_pkStderrFile);
		ms_pkStderrFile = NULL;
	}

	if (ms_pkStdoutFile)
	{
		fclose(ms_pkStdoutFile);
		ms_pkStdoutFile = NULL;
	}
}

void ntConsole::update( float fDeltaTime )
{
}

void ntConsole::open()
{
	AllocConsole();

	ms_hScreenBuffer = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ | FILE_SHARE_WRITE,
		NULL,
		CONSOLE_TEXTMODE_BUFFER,
		NULL);

	SetConsoleActiveScreenBuffer(ms_hScreenBuffer);

	COORD kCoord;
	kCoord.X = 60;
	kCoord.Y = 160;
	SetConsoleScreenBufferSize(ms_hScreenBuffer, kCoord);

	close();

	freopen_s(&ms_pkStderrFile, "CONOUT$", "wt", stderr);
	freopen_s(&ms_pkStdoutFile, "CONOUT$", "wt", stdout);
}

void ntConsole::start()
{
	open();
}
