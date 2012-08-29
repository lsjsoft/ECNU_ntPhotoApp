#pragma once
#include "ntServiceBase.h"
#include "ntTexture.h"
#include "ntHistogram.h"

class ntPlugBaseService: public ntServiceBase
{
public:
	ntImplService(ntPlugBaseService);
	ntPlugBaseService(void);
	~ntPlugBaseService(void);

	bool makeGray(ntTexture32Ptr tex);
	bool makeInvert(ntTexture32Ptr tex);

	ntHistogramPtr getHistogram(ntTexture32Ptr tex);
};
