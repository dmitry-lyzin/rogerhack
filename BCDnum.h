#pragma once
#include "common.h"

extern char dec_separator;

//**class BCDnum <Type, leading_char, length, dec_separator, precision, trailing_char>
/**class BCDnum <Type, leading_char, length, precision, trailing_char>
 * . Шаблон класса двоично десятичных (BCD) чисел
 * 
 * @param typename Type          тип поля для хранения BCD числа
 * @param char     leading_char  ведущий символ, если 0x0 - ничего не выводить, число прижмётся к левому краю
 * @param size_t   length        общая длина числа без символа-разделитель целой и дробной частей
 * @param- char     dec_separator символ-разделитель целой и дробной частей
 * @param size_t   precision     точность (длина дробной части)
 * @param char     trailing_char символ вместо завершающих нулей, если 0x0 - ничего не выводить
 */
template
<	typename Type
,	const char	leading_char
,	const size_t	length
/*,	const char	dec_separator*/
,	const size_t	precision
,	const char	trailing_char='0'
> class BCDnum
{
protected:
	Type value;
private:
	static constexpr const size_t	shift	= (length - 1) * 4;
	static constexpr const Type	mask	= 0xf << shift;
public:
	void read ( std::istream& input  )
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
		input >> (const char)dec_separator;

		for( ; i > 0; --i )
		{
			if( !(input >>= dc) )
				break;

			value <<= 4;
			value |= dc.value;
		}
		value <<= (4 * i);
	}
	void print( std::ostream& output ) const
	{
		Type tmp = value;
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
			if( leading_char )
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

		output.put( dec_separator );
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
	friend std::ostream& operator<<( std::ostream &os, const BCDnum &obj ) { obj.print( os); return os;}
	friend std::istream& operator>>( std::istream &is,       BCDnum &obj ) { obj.read( is); return is; }
};
