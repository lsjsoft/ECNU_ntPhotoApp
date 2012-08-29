#pragma once
#include "ntCounter.h"


class ntHistogram: public ntCountedObject
{

public:
	enum { MAX_COLOR = 256, };
	ntHistogram(void);
	~ntHistogram(void);

	void full(unsigned int p[MAX_COLOR], unsigned int* pBuff, unsigned int uiLength);

	unsigned int r[MAX_COLOR];
	unsigned int g[MAX_COLOR];
	unsigned int b[MAX_COLOR];
	unsigned int rgb[MAX_COLOR]; 

	unsigned int getMax()
	{
		unsigned int uMax = 0;
		_getMax(r, uMax);
		_getMax(g, uMax);
		_getMax(b, uMax);
		_getMax(rgb, uMax);
		return uMax;
	}

protected:
	void _getMax(unsigned int p[MAX_COLOR], unsigned int& k)
	{
		for(unsigned int i=0; i<MAX_COLOR; ++i)
		{
			p[i] > k ? k = p[i] : 0;
		}
	}
};

typedef ntCountedPtr<ntHistogram> ntHistogramPtr;
