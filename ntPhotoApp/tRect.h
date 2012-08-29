#pragma once

template<typename T>
class tRect
{
public:
	tRect(void)
	: x(0)
	, y(0)
	, w(0)
	, h(0)
	{
	}

	tRect(T ax, T ay, T aw, T ah)
	: x(ax)
	, y(ay)
	, w(aw)
	, h(ah) 
	{
	}

	~tRect(void) 
	{
	}

	T width() const 
	{
		return w; 
	}

	T height() const 
	{
		return h; 
	}

	T top() const 
	{ 
		return y;
	}

	T left() const 
	{
		return x;
	}

	T right() const 
	{
		return x + w; 
	}

	T bottom() const 
	{ 
		return y + h;
	}

	tPoint<T> getLeftTop()
	{
		return tPoint<T>(x, y);
	}

	tPoint<T> getRightTop()
	{
		return tPoint<T>(x+w, y);
	}

	bool operator == ( const tRect& a)
	{
		return x == a.x 
			&& y == a.y
			&& w == a.w 
			&& h == a.h;
	}

	bool operator != ( const tRect& a)
	{
		return !( *this == a );
	}

	bool testInRect(const tRect& rkSubRect) const
	{
		return (
			rkSubRect.x >= x &&
			rkSubRect.y >= y &&
			rkSubRect.x+rkSubRect.w <= x+w &&
			rkSubRect.y+rkSubRect.h <= y+h );
	}

	bool getIntersection(const tRect& rkRect, tRect& rResult) const
	{
		T sRight = x + w;
		T sBottom = y + h;
		T cRight = rkRect.x + rkRect.w;
		T cBottom = rkRect.y + rkRect.h;

		if (x > cRight || y > cBottom || sRight < rkRect.x || sBottom < rkRect.y)
		{
			return false;
		}

		rResult.x = x < rkRect.x? rkRect.x : x;
		rResult.y = y < rkRect.y ? rkRect.y : y;
		rResult.w = sRight > cRight ? cRight - rResult.x : sRight - rResult.x;
		rResult.h = sBottom > cBottom ? cBottom - rResult.y : sBottom - rResult.y;

		return true;
	}

	void getUnion(const tRect& rkRect, tRect& kResult) const
	{
		float sRight = x + w;
		float sBottom = y + h;
		float cRight = rkRect.x + rkRect.w;
		float cBottom = rkRect.y + rkRect.h;
		kResult.x = x > rkRect.x ? rkRect.x : x;
		kResult.y = y > rkRect.y ? rkRect.y : y;
		kResult.w = sRight < cRight ? cRight-kResult.x : sRight-kResult.x;
		kResult.h = sBottom < cBottom ? cBottom-kResult.y : sBottom-kResult.y;
	}

	T x, y, w, h;
};

//////////////////////////////////////////////////////////////////////////

typedef tRect<float> ntRectf;
typedef tRect<int> ntRecti;
typedef tRect<unsigned int> ntRectui;

#define noClipRecti ntRecti(0,0,1024*1024, 1024*1024)

class ntWindowRect: public RECT
{
public:
	ntWindowRect() {}
	~ntWindowRect() {}

	int width() const { return right- left; }
	int height() const { return bottom - top; }
	void setWidthByLeft(int w) { right= left + w; }
	void setHeightByTop(int h) { bottom = top + h ; }
};
