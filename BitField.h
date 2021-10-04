#pragma once
#include "common.h"

//----------------------------------------------------------------
class BitField
{
	uint8_t* operand;
	const char** state_names;
	const size_t state_names_len;
	const size_t bitindex;
	const uint8_t bitmask;

public:
	BitField( const char* state_names_[], size_t state_names_len_, unsigned int bitindex_ )
		: operand( NULL), state_names( state_names_ ), state_names_len( state_names_len_ )
		, bitindex( bitindex_ ), bitmask( (state_names_len_ - 1u) << bitindex_ )
	{};

	BitField& operator()( const uint8_t* operand_ ) { operand = (uint8_t*) operand_; return *this; };

	void read( std::istream& input );
	void print( std::ostream& output ) const;
};

//----------------------------------------------------------------
inline void BitField::print( std::ostream& output ) const
{
	output << state_names[((*operand) & bitmask) >> bitindex];
}

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, BitField& s )
{
	s.read( input ); return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, BitField& s )
{
	s.print( output ); return output;
}
