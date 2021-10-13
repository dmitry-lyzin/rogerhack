#pragma once
#include "common.h"
#include "assert.h"

#define TEMPL template		\
< typename T			\
, const char* state_names[]	\
, const size_t state_names_len	\
, const size_t bitindex		\
>
#define THIS BitField< T, state_names, state_names_len, bitindex>
// BitField< T, state_names, state_names_len, bitindex>
// state_names_len должен == степени двойки
TEMPL class BitField
{
protected:
	T value;
private:
	static_assert(!(state_names_len & (state_names_len - 1)), "The state_names_len is a NOT A power of two");
	static constexpr const T bitmask = (state_names_len - 1) << bitindex;
public:
	void read ( std::istream& input  );
	void print( std::ostream& output ) const;
};
TEMPL inline std::istream& operator>>( std::istream& input,        THIS& s ) { s.read ( input ); return input;  }
TEMPL inline std::ostream& operator<<( std::ostream& output, const THIS& s ) { s.print( output); return output; }

TEMPL        void THIS::read ( std::istream& input  )
{
	for( size_t i = 0; i < state_names_len; i++ )
	{
		if( !state_names[i] )
			continue;
		if( input >>= state_names[i] )
		{
			value &= ~bitmask;
			value |= T(i) << bitindex;
			return;
		}
	}

	const char* t = "Wrong read! Wait words: \"";
	for( size_t i = 0; i < state_names_len; i++ )
	{
		if( !state_names[i] )
			continue;
		cerr << t << state_names[i] << '"';
		t = " or \"";
	}
	streamerror( input );
}
TEMPL inline void THIS::print( std::ostream& output ) const
{
	output << state_names[(value & bitmask) >> bitindex];
};

#undef TEMPL
#undef THIS
