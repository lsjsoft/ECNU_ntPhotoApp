#pragma once

template<typename T>
class tPoint
{
public:
	tPoint(void): x(0), y(0) {}
	~tPoint(void) {}
	tPoint(T ax, T ay):x(ax), y(ay) {}

	T x, y;
};

//////////////////////////////////////////////////////////////////////////

typedef tPoint<float> ntPointf;
typedef tPoint<int> ntPointi;
typedef tPoint<unsigned int> ntPointui;
