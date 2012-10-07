#include "StdAfx.h"
#include "ntOpNone.h"

ntOpNone::ntOpNone(void)
: ntOp(ntOpNone::_GetOpId())
{
}

ntOpNone::~ntOpNone(void)
{
}

opIdType ntOpNone::_GetOpId()
{
	return "None";
}
