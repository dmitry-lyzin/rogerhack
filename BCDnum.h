#pragma once
#include "common.h"

#define TEMPL template< typename T, const char Fmt[]>
#define THIS  BCDnum <T, Fmt>

/* Описание Fmt
	Fmt[0] - ведущий символ, если '-' ничего не выводить, число прижмётся к левому краю
	Fmt[1] - общая длина числа без символа-разделитель целой и дробной частей
	Fmt[2] - символ-разделитель целой и дробной частей
	Fmt[3] - точность (длина дробной части)
	Fmt[4] - символ вместо завершающих нулей, если '\0' ничего не выводить
*/
TEMPL class BCDnum
{
protected:
	T value;
private:
	static constexpr const char	leading_char	= Fmt[0];
	static constexpr const size_t	length		= Fmt[1] - '0';
	static constexpr const char	dot_char	= Fmt[2];
	static constexpr const size_t	precision	= Fmt[3] - '0';
	static constexpr const char	trailing_char	= Fmt[4];

	static constexpr const size_t	shift	= (length - 1) * 4;
	static constexpr const T	mask	= 0xfu << shift;

public:
	void read ( std::istream& input );
	void print( std::ostream& output ) const;
};

//----------------------------------------------------------------
TEMPL void THIS::read( std::istream& input )
{
	Digit_char dc;
	input >> dc;
	value = dc.value;
	while( input >>= dc )
	{
		value <<= 4;
		value |= dc.value;
	}

	size_t i = precision;
	if( !precision )
		return;
	input >> dot_char;

	for( ; i > 0; --i )
	{
		if( !(input >>= dc) )
			break;

		value <<= 4;
		value |= dc.value;
	}
	value <<= (4 * i);

}
TEMPL void THIS::print( std::ostream& output ) const
{
	T tmp = value;
	char c;
	size_t i = length - precision - 1;
	for( ; i > 0; --i )
	{
		c = (tmp & mask) >> shift;
		tmp <<= 4;
		if( c )
		{
			output.put( c + '0' );
			--i;
			break;
		}
		if( leading_char != '-' )
			output.put( leading_char );
	}

	for( ; i > 0; --i )
	{
		c = (tmp & mask) >> shift;
		tmp <<= 4;
		output.put( c + '0' );
	}

	c = (tmp & mask) >> shift;
	tmp <<= 4;
	output.put( c + '0' );

	if( !precision )
		return;

	output.put( dot_char );
	for( size_t i = precision; i > 0; --i )
	{
		if( !tmp )
		{
			if( !trailing_char )
				break;
			for( ; i > 0; --i )
				output.put( trailing_char );
			break;
		}
		c = (tmp & mask) >> shift;
		tmp <<= 4;
		output.put( c + '0' );
	}
}

//----------------------------------------------------------------
TEMPL inline std::istream& operator>>( std::istream& input,	   THIS& s ) { s.read ( input ); return input; }
TEMPL inline std::ostream& operator<<( std::ostream& output, const THIS  s ) { s.print( output); return output; }

#undef TEMPL
