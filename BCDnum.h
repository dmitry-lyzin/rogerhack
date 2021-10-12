#pragma once
#include "common.h"

#define TEMPL template		\
< typename T			\
, const char	leading_char	\
, const size_t	length		\
, const char	dot_char	\
, const size_t	precision	\
, const char	trailing_char	\
>
#define THIS BCDnum <T, leading_char, length, dot_char, precision, trailing_char>

/** ������ ������ ������� ���������� (BCD) �����
 * 
 * @param typename T             ��� ���� ��� �������� BCD �����
 * @param char     leading_char  ������� ������, ���� '-' ������ �� ��������, ����� �������� � ������ ����
 * @param size_t   length        ����� ����� ����� ��� �������-����������� ����� � ������� ������
 * @param char     dot_char      ������-����������� ����� � ������� ������
 * @param size_t   precision     �������� (����� ������� �����)
 * @param char     trailing_char ������ ������ ����������� �����, ���� '\0' ������ �� ��������
 */
TEMPL class BCDnum
{
protected:
	T value;
private:
	static constexpr const size_t	shift	= (length - 1) * 4;
	static constexpr const T	mask	= 0xf << shift;
public:
	void read ( std::istream& input );
	void print( std::ostream& output ) const;
};
TEMPL inline std::istream& operator>>( std::istream& input,        THIS& s ) { s.read ( input );  return input;  }
TEMPL inline std::ostream& operator<<( std::ostream& output, const THIS& s ) { s.print( output ); return output; }


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

#undef TEMPL
#undef THIS
