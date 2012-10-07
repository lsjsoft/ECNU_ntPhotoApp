#pragma once

#include <math.h>

template<typename T>
class tPoint
{
public:
	tPoint(void): x(0), y(0) {}
	~tPoint(void) {}
	tPoint(T ax, T ay):x(ax), y(ay) {}

	T length(const tPoint& a)
	{
		T vx= a.x-x;
		T vy= a.y-y;
		return sqrtf(vx*vx+vy*vy);
	}

	T x, y;
};

//////////////////////////////////////////////////////////////////////////

typedef tPoint<float> ntPointf;
typedef tPoint<int> ntPointi;
typedef tPoint<unsigned int> ntPointui;
