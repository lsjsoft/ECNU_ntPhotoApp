#pragma once
#include "ntOp.h"

class ntOpNone: public ntOp
{
public:
	ntOpNone(void);
	~ntOpNone(void);

	static opIdType _GetOpId();
};
