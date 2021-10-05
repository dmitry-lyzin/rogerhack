#pragma once
#include "common.h"

//----------------------------------------------------------------
class BitField
{
	union
	{
		uint8_t *pointer;
		uint8_t constant;
	} operand;

	const char** state_names;
	const size_t state_names_len;
	const size_t bitindex;
	const uint8_t bitmask;

public:
	BitField( const char* state_names_[], size_t state_names_len_, unsigned int bitindex_ )
		: operand( { NULL } ), state_names( state_names_ )
		, state_names_len( state_names_len_ ), bitindex( bitindex_ )
		, bitmask( (state_names_len_ - 1u) << bitindex_ )
	{
		assert	(  b00000010 == state_names_len_ 
			|| b00000100 == state_names_len_
			|| b00001000 == state_names_len_
			|| b00010000 == state_names_len_
			|| b00100000 == state_names_len_
			);
	};

	BitField& operator()(       uint8_t *operand_) { operand.pointer  = operand_; return *this; };
	BitField& operator()( const uint8_t  operand_) { operand.constant = operand_; return *this; };

	SAMPLE_CLASS
( BitField )


//----------------------------------------------------------------
inline void BitField::print( std::ostream& output ) const
{
	output << state_names[ (operand.constant & bitmask) >> bitindex];
}
