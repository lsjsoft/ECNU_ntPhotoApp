#pragma once

#include <string>
#include <vector>
#include <list>
#include <stdio.h>
#include <stdlib.h>
#include <io.h>
#include <algorithm>
#include <cctype>
#include <locale>
#include <map>

#ifdef _DEBUG_MEM
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#define ntNew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define ntNew new
#endif

#pragma warning(disable: 4996)

#include "ntCounter.h"
#include "ntDefine.h"
#include "tPoint.h"
#include "tRect.h"
#include "ntPixel24.h"
#include "ntPixel32.h"
