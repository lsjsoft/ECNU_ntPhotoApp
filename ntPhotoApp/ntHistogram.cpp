#include "StdAfx.h"
#include "ntHistogram.h"
#include <math.h>

ntHistogram::ntHistogram(void)
{
	ntMemset(r);
	ntMemset(g);
	ntMemset(b);
	ntMemset(rgb);
}

ntHistogram::~ntHistogram(void)
{
}

void ntHistogram::full(unsigned int p[MAX_COLOR],
					   unsigned int* pBuff, 
					   unsigned int uiLength )
{
	if ( uiLength == MAX_COLOR)
	{
		memcpy(pBuff, p, sizeof(unsigned int)*uiLength);
	}
	else
	{
		memset(p, 0, sizeof(p));
	}

/*	else if ( uiLength > MAX_COLOR)
	{
		float f = MAX_COLOR / (float)uiLength;

		/// 合并
		for(unsigned int i=0; i<uiLength; ++i)
		{
			unsigned int idx = static_cast<unsigned int >( i * f );
			pBuff[i] = (unsigned int)(sqrtf( p[idx] ));
		}
	}
	else
	{
		/// 采样
		unsigned int n = static_cast<unsigned int >
			( MAX_COLOR / (float)uiLength);

		for(unsigned int i=0; i<uiLength; ++i)
		{
			unsigned int uTotal = 0;
			for(unsigned int j=i*n; j<(i+1)*n && j<MAX_COLOR; ++j)
			{
				uTotal+= p[j];
			}
			uTotal /= n;

			pBuff[i] = sqrtf(uTotal);
		}
	}
*/
}
