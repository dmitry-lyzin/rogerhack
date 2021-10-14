#include "Tone.h"

constexpr uint16_t digital_tone_bit = 0b10000000 << 8;

//----------------------------------------------------------------
void Tone::read( std::istream& input )
{
	if( input >>= "OFF" )
	{
		u.bin = 0xFFFFu;
		return;
	}

	if( input >>= 'D' )
	{
		input >> u.digital_tone >> u.polarity_bit;
		u.bin |= digital_tone_bit;
		return;
	}

	input >> u.analog_tone;
}

//----------------------------------------------------------------
void Tone::print( std::ostream& output ) const
{
	if( 0xFFFFu == u.bin )
	{
		output << "OFF";
		return;
	}
	if( u.bin & digital_tone_bit )
	{
		output << 'D' << u.digital_tone << u.polarity_bit;
		return;
	}
	output << u.analog_tone;
}
