#pragma once
#include "common.h"

#define TEMPL template		\
< typename	Type		\
, const size_t	bitindex	\
, typename	Sample		\
, Sample	samples[]	\
, const size_t	samples_len	\
>
#define THIS BitField< Type, bitindex, Sample, samples, samples_len>

/* BitField< Type, samples, samples_len, bitindex>
 * @param typename	Type		тип в котором находится битовое поле
 * @param size_t	bitindex	номер бита(ов), считая от 0
 * @param typename	Sample		тип шаблона (const char* | const char)
 * @param Sample_type	samples[]	массив шаблонов имен значений битово поля
 * @param size_t	samples_len	длина массива шаблонов == степени двойки
 */
TEMPL class BitField
{
	static_assert( !(samples_len & (samples_len - 1)), "The samples_len is a NOT a power of two");
protected:
	Type value;
private:
    //static constexpr const size_t	bitindex = bitindex;
	static constexpr const Type	bitmask  = (samples_len - 1) << bitindex;
public:
	operator Type() const	{ return (value & bitmask) >> bitindex;	};
	operator bool() const	{ return  value & bitmask;		};
	friend inline BitField& operator |= ( BitField& left, const Type right )
	{
		//left.value &= ~left.bitmask;
		left.value |= right << left.bitindex;
		return left;
	};
	bool mayread	( std::istream& input );
	void read	( std::istream& input  );
	void print	( std::ostream& output ) const
	{
		output << samples[ (value & bitmask) >> bitindex];
	};
};

//----------------------------------------------------------------
TEMPL inline bool	   operator>>=( std::istream& input,        THIS& s ) { return s.mayread( input );	  }
TEMPL inline std::istream& operator>> ( std::istream& input,        THIS& s ) { s.read ( input ); return input;	  }
TEMPL inline std::ostream& operator<< ( std::ostream& output, const THIS& s ) { s.print( output ); return output; }

//----------------------------------------------------------------
TEMPL bool THIS::mayread( std::istream& input )
{
	for( Type i = 0; i < samples_len; i++ )
	{
		if( !samples[i] )
			continue;
		if( input >>= samples[i] )
		{
			value &= ~bitmask;
			value |= i << bitindex;
			return true;
		}
	}
	return false;
}

//----------------------------------------------------------------
TEMPL void THIS::read( std::istream& input )
{
	if( mayread( input ) )
		return;

	cerr << filename << ": Wrong read! Wait words: ";
	const char* t = "\"";
	for( size_t i = 0; i < samples_len; i++ )
	{
		if( !samples[i] )
			continue;
		cerr << t << samples[i] << '"';
		t = " or \"";
	}
	streamerror( input );
}

#undef TEMPL
#undef THIS
