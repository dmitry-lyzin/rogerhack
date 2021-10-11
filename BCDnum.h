#pragma once
#include "common.h"

#define TEMPL template< typename T, const char Fmt[]>
#define THIS  BCDnum< T, Fmt>
/* Описание Fmt
	Fmt[0] - ведущий символ, если '-' ничего не выводить, число прижмётся к левому краю
	Fmt[1] - символ-разделитель целой и дробной частей
	Fmt[2] - точность (длина дробной части)
	Fmt[3] - завершающий символ, если '-' ничего не выводить
*/
TEMPL class BCDnum
{
protected:
	T value;
private:
	enum { bitshift = sizeof( value ) * 8 - 4 };

	static constexpr const char	leading_char	= Fmt[0];
	static constexpr const char	dot_char	= Fmt[1];
	static constexpr const size_t	precision	= Fmt[2] - '0';
	static constexpr const char	trailing_char	= Fmt[3];
	static constexpr const T	mask = 0xfu << THIS::bitshift;

public:
	BCDnum( const T value_) : value( value_) {};

	void read ( std::istream& input );									
	void print( std::ostream& output ) const;								
};

//----------------------------------------------------------------
TEMPL void THIS::read( std::istream& input )
{
	Digit_char c;
	input >> c;
	value = c;
	while( input >>= c )
	{
		value <<= 4;
		value |= c;
	}
	input >> dot_char;

	size_t i = precision;
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
TEMPL void THIS::print( std::ostream& output ) const
{
	T tmp = value;
	char lc = leading_char;
	for( size_t i = sizeof( value ) * 2; i > 0; --i )
	{
		if( precision == i )
			output.put( dot_char );

		char c = (tmp & mask) >> bitshift;
		tmp <<= 4;
		if( c )
		{
			output.put( c + '0' );
			lc = '\0';
			continue;
		}
		switch( lc )
		{
		case '\0':	output.put( '0');	break;
		case '-':				break;
		default:	output.put( lc );	break;
		};
	}
}

//----------------------------------------------------------------
TEMPL inline std::istream& operator>>( std::istream& input,	   THIS& s ) { s.read ( input ); return input; }
TEMPL inline std::ostream& operator<<( std::ostream& output, const THIS  s ) { s.print( output); return output; }

#undef TEMPL
