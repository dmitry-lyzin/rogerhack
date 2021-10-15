#pragma once
#include "common.h"


#define TEMPL template	\
< typename Value	\
, typename Sample	\
>

#define THIS BitField< Value, Sample>

//----------------------------------------------------------------
TEMPL class BitField
{
	union
	{
		Value *pointer;
		Value constant;
	} value;

	Sample		*sample;
	const size_t	sample_len;
	const size_t	bitindex;
	const Value	bitmask;

public:
	BitField( Sample sample_[], uint8_t sample_len_, size_t bitindex_ )
		: value( { NULL } ), sample( sample_ )
		, sample_len( sample_len_ ), bitindex( bitindex_ )
		, bitmask( (sample_len_ - 1) << bitindex_ )
	{
		assert( (!(sample_len & (sample_len - 1))
			, "The sample_len is a NOT a power of two"
			) );
	};

	BitField& operator()(       Value *operand_ ) { value.pointer  = operand_; return *this; };
	BitField& operator()( const Value  operand_ ) { value.constant = operand_; return *this; };

	operator bool() const	{ return value.constant & bitmask; };
	void set()		{ *value.pointer |= bitmask; };
	bool mayread( std::istream& input );
	void read( std::istream& input );
	void print( std::ostream& output ) const
	{
		output << sample[(value.constant & bitmask) >> bitindex];
	};
};

TEMPL inline bool	   operator>>=( std::istream& input,        THIS& s ) { return s.mayread( input );        }
TEMPL inline std::istream& operator>> ( std::istream& input,        THIS& s ) { s.read ( input  ); return input;  }
TEMPL inline std::ostream& operator<< ( std::ostream& output, const THIS& s ) { s.print( output ); return output; }

//----------------------------------------------------------------
TEMPL bool THIS::mayread( std::istream& input )
{
	for( size_t i = 0; i < sample_len; i++ )
	{
		if( !sample[i] )
			continue;
		if( input >>= sample[i] )
		{
			register auto r = *value.pointer;
			r &= ~bitmask;
			r |= Value(i) << bitindex;
			*value.pointer = r;
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
	for( size_t i = 0; i < sample_len; i++ )
	{
		if( !sample[i] )
			continue;
		cerr << t << sample[i] << '"';
		t = " or \"";
	}
	streamerror( input );
}

//----------------------------------------------------------------
#undef TEMPL
#undef THIS
