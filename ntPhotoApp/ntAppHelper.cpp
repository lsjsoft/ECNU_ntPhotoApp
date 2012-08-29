#include "StdAfx.h"
#include "ntAppHelper.h"

std::string ntGetShortName(const std::string& rkpathName)
{
	size_t p = rkpathName.rfind('\\');
	if ( p == std::string::npos)
	{
		return rkpathName;
	}

	return rkpathName.substr(p+1);
}
