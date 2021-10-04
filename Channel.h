#pragma once
#include "common.h"
#include "Frequency.h"
#include "Tone.h"
#include "Flags.h"

//----------------------------------------------------------------
class Channel
{
	Frequency receive;
	Frequency transmit;
	Tone rx_tone;
	Tone tx_tone;
	Flags flags;
	uint8_t x01;	// хз что, там лежит 0x01
	uint8_t x01_;	// хз что, там лежит 0x01
	uint8_t xFF;	// хз что, там лежит 0xFF

public:
	void read( std::istream& input );
	void print( std::ostream& output ) const;
};

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Channel& s )
{
	s.read( input ); return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, const Channel& s )
{
	s.print( output );	return output;
}
