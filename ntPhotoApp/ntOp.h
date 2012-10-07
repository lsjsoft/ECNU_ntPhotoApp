#pragma once
#include "ntMsgHandle.h"

typedef std::string opIdType;

class ntOp: public ntMsgHandle
{
public:
	ntOp(opIdType opId);
	virtual ~ntOp(void);

	const opIdType& GetOpId() const { return m_opId; }

	virtual void onEnter() {}
	virtual void onLeave() {}

	virtual void onInit() {}
	virtual void onRelease() {}

	virtual HCURSOR getCursor() { return 0; }

protected:
	opIdType m_opId;
};
