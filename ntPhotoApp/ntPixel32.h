#pragma once


#pragma pack(push, 1)

class ntPixel32 : public ntPixel24
{
public:
	unsigned char alpha;

	// Constructors
	ntPixel32()
	{
	}

	ntPixel32( unsigned char new_red, 
		unsigned char new_green, 
		unsigned char new_blue, 
		unsigned char new_alpha = 255 )
	: ntPixel24( new_red, new_green, new_blue )
	, alpha( new_alpha )
	{
	}

	ntPixel32( ntPixel24 const& arg )
	: ntPixel24( arg )
	, alpha(255)
	{
	}

	ntPixel32( unsigned int arg ) 
	{
		*(reinterpret_cast<unsigned int*>(this)) = arg;
	}

	// Operators
	ntPixel32 & operator&=( ntPixel32 right );
	ntPixel32 & operator|=( ntPixel32 right );
	ntPixel32 & operator^=( ntPixel32 right );

	operator unsigned int() const
	{	
		return *(reinterpret_cast<unsigned int const*>(this));
	}

	void blend(const ntPixel32* src)
	{
		//C = C0 * ( 1 - A1 ) + C1 * A1 
		unsigned char ia = 0xFF - src->alpha;
		red = ( red * ia + src->red * src->alpha ) >> 8;
		green = ( green * ia + src->green * src->alpha ) >> 8;
		blue = ( blue * ia + src->blue * src->alpha ) >> 8;
	}

	unsigned char getLuminance() const
	{
		return ( red * 76 + green * 150 + blue * 29 ) >> 8;	
	}

	void fromCOLORREF(COLORREF c)
	{
		red = (unsigned char)(c & 0xFF);
		green = (unsigned char)( (c >> 8 ) & 0xFF);
		blue = (unsigned char)( (c >> 16 ) & 0xFF);
	}
};

#pragma pack(pop)

inline ntPixel32 & ntPixel32::operator&=( ntPixel32 right )
{
	*(reinterpret_cast<unsigned int*>(this)) &= right;
	return *this;
}

inline ntPixel32 & ntPixel32::operator|=( ntPixel32 right )
{
	*(reinterpret_cast<unsigned int*>(this)) |= right;
	return *this;
}

inline ntPixel32 & ntPixel32::operator^=( ntPixel32 right )
{
	*(reinterpret_cast<unsigned int*>(this)) ^= right;
	return *this;
}

