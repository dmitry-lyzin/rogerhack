#include "Tone.h"

#define digital_tone_bit		(b10000000 << 8)
#define digital_tone_polarity_bit	(b01000000 << 8)

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
		input >> u.digital_tone;

		u.bin |= digital_tone_bit;
		switch( input.get() )
		{
		case 'I':
			u.bin |= digital_tone_polarity_bit;
			break;
		case 'N':
			u.bin &= ~digital_tone_polarity_bit;
			break;
		default:
			input.unget();
			streamerror( input );
			break;
		}
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
	if( digital_tone_bit & u.bin )
	{
		output	<< "D" << u.digital_tone
			<< (u.bin & digital_tone_polarity_bit ? 'I' : 'N');
		return;
	}
	output << u.analog_tone;
}
