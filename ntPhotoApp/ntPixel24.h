#pragma once

#pragma pack(push, 1)

class ntPixel24  
{
public:
	unsigned char blue;
	unsigned char green;
	unsigned char red;

	inline ntPixel24(){};
	explicit ntPixel24( unsigned int arg );
	ntPixel24( unsigned char new_red, unsigned char new_green, unsigned char new_blue );

	ntPixel24 & operator&=( ntPixel24 right );
	ntPixel24 & operator|=( ntPixel24 right );
	ntPixel24 & operator^=( ntPixel24 right );

	operator unsigned int() const;
};

#pragma pack(pop)

inline ntPixel24::ntPixel24( unsigned int arg )
{
	blue  = static_cast<unsigned char>(arg);
	green = static_cast<unsigned char>(arg >> 8);
	red   = static_cast<unsigned char>(arg >> 16);
}

inline ntPixel24::ntPixel24( unsigned char new_red, unsigned char new_green, unsigned char new_blue )
:	blue( new_blue ), green( new_green ), red( new_red )
{
}

inline ntPixel24 & ntPixel24::operator&=( ntPixel24 right )
{
	blue &= right.blue;
	green &= right.green;
	red &= right.red;
	return *this;
}

inline ntPixel24 & ntPixel24::operator|=( ntPixel24 right )
{
	blue |= right.blue;
	green |= right.green;
	red |= right.red;
	return *this;
}

inline ntPixel24 & ntPixel24::operator^=( ntPixel24 right )
{
	blue ^= right.blue;
	green ^= right.green;
	red ^= right.red;
	return *this;
}

inline ntPixel24::operator unsigned int() const
{
	return blue | (green << 8) | (red << 16);
}
