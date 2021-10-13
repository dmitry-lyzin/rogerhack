#pragma once
#include "common.h"
#include "assert.h"

#define TEMPL template		\
< typename	Type		\
, typename	Sample_type	\
, Sample_type	samples[]	\
, const size_t	samples_len	\
, const size_t	bitindex	\
>
#define THIS BitField< Type, Sample_type, samples, samples_len, bitindex>

/* BitField< Type, samples, samples_len, bitindex>
 * @param typename	Type		тип поля для хранения ?
 * @param typename	Sample_type	тип шаблона (const char* | const char)
 * @param Sample_type	samples[]	массив шаблонов
 * @param size_t	samples_len	длина массива шаблонов == степени двойки
 * @param size_t	bitindex	номер бита, считая от 0
 */
TEMPL class BitField
{
	static_assert( !(samples_len& (samples_len - 1)), "The samples_len is a NOT a power of two");
protected:
	Type value;
private:
	static constexpr const size_t	bitindex = bitindex;
	static constexpr const Type	bitmask  = (samples_len - 1) << bitindex;
public:
	operator Type() const	{ return (value & bitmask) >> bitindex;	};
	operator bool() const	{ return  value & bitmask;		};
	friend inline BitField& operator |= ( BitField& left, const unsigned int right )
	{
		//left.value &= ~left.bitmask;
		left.value |= Type( right) << left.bitindex;
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
	for( size_t i = 0; i < samples_len; i++ )
	{
		if( !samples[i] )
			continue;
		if( input >>= samples[i] )
		{
			value &= ~bitmask;
			value |= Type( i ) << bitindex;
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

	const char* t = "Wrong read! Wait words: \"";
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
