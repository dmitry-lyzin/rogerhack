#pragma once
#include "common.h"

//----------------------------------------------------------------
template< typename T, size_t Num_after_dot, bool Trailing_null>
class BCDnum
{
	T value;

	static const bool trailing_null = false;
	static const size_t num_after_dot;
	static const T mask;
	enum { bitshift = sizeof( value ) * 8 - 4 };

public:
	BCDnum( const T value_) : value( value_) {};
	operator T() const { return value; };
	void read( std::istream& input );									
	void print( std::ostream& output ) const;								
};														

template< typename T, size_t Num_after_dot, bool Trailing_null>
const bool BCDnum< T, Num_after_dot, Trailing_null>::trailing_null = Trailing_null;

template< typename T, size_t Num_after_dot, bool Trailing_null>
const size_t BCDnum< T, Num_after_dot, Trailing_null>::num_after_dot = Num_after_dot;

template< typename T, size_t Num_after_dot, bool Trailing_null>
const T BCDnum< T, Num_after_dot, Trailing_null>::mask = 0xfu << BCDnum< T, Num_after_dot, Trailing_null>::bitshift;

//----------------------------------------------------------------
template< typename T, size_t Num_after_dot, bool Trailing_null>
void BCDnum< T, Num_after_dot, Trailing_null>::read( std::istream& input )
{
	Digit_char c;
	input >> c;
	value = c;
	while( input >>= c )
	{
		value <<= 4;
		value |= c;
	}
	input >> '.';

	size_t i = num_after_dot;
	for( ; i > 0; --i )
	{
		if( !(input >>= c) )
			break;

		value <<= 4;
		value |= c;
	}
	value <<= (4 * i);
}

//----------------------------------------------------------------
template< typename T, size_t Num_after_dot, bool Trailing_null>
void BCDnum< T, Num_after_dot, Trailing_null>::print( std::ostream& output ) const
{
	T tmp = value;
	bool tn = trailing_null;
	for( size_t i = sizeof( value ) * 2; i > 0; --i )
	{
		if( num_after_dot == i )
			output.put( '.' );
		char c = (tmp & mask) >> bitshift;
		tmp <<= 4;
		if( tn || c )
		{
			output.put( c + '0' );
			tn = true;
		}
	}
}

//----------------------------------------------------------------
template< typename T, size_t Num_after_dot, bool Trailing_null>
inline std::istream& operator>>( std::istream& input,		BCDnum< T, Num_after_dot, Trailing_null>& s ) { s.read ( input ); return input; }
template< typename T, size_t Num_after_dot, bool Trailing_null>
inline std::ostream& operator<<( std::ostream& output, const	BCDnum< T, Num_after_dot, Trailing_null>  s ) { s.print( output); return output; }
