#include "Tone.h"

const char polarity_bit2str[] = { 'N', 'I' };
BitField< uint16_t, const char> polarity_bit( PS( polarity_bit2str ), 14 );

const uint16_t digital_tone_bit = 1 << 15;

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
		input >> u.digital_tone >> polarity_bit( &u.bin );
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
		output << 'D' << u.digital_tone << polarity_bit( u.bin );
		return;
	}
	output << u.analog_tone;
}
