#pragma once

class ntServiceBase
{
public:
	ntImplService(ntServiceBase);
	ntServiceBase(void);
	virtual ~ntServiceBase(void);
	virtual void start() {}
	virtual void end() {}
	virtual void update(float fDeltaTime) {}
};
