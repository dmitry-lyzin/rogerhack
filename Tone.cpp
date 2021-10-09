#include "Tone.h"

#define digital_tone_bit b10000000
#define digital_tone_polarity_bit b01000000

//----------------------------------------------------------------
void Tone::read( std::istream& input )
{
	if( input >>= "OFF" )
	{
		b[0] = 0xFFu;
		b[1] = 0xFFu;
		return;
	}

	if( input >>= 'D' )
	{
		b[1]  = getdigit( input );
		b[1] |= digital_tone_bit;
		b[0]  = getdigit( input ) << 4;
		b[0] |= getdigit( input );

		switch( input.get() )
		{
		case 'I':
			b[1] |= digital_tone_polarity_bit;
			break;
		case 'N':
			b[1] &= ~digital_tone_polarity_bit;
			break;
		default:
			input.unget();
			streamerror( input );
			break;
		}
		return;
	}

	char buf[8] = { 0 };
	char* p = buf + 4;

	*p++ = getdigit( input );

	char c;
	while( input.get( c ) )
	{
		if( !isdigit( c ) )
		{
			input.unget();
			break;
		}
		if( p < buf + std::size( buf ) )
			*p++ = c - '0';
	}

	input >> '.';

	b[0]  = getdigit( input );
	b[0] |= (*--p) << 4;
	b[1]  = (*--p);
	b[1] |= (*--p) << 4;
}

//----------------------------------------------------------------
void Tone::print( std::ostream& output ) const
{
	if( 0xFFu == b[1] && 0xFFu == b[0] )
	{
		output << "OFF";
		return;
	}
	if( digital_tone_bit & b[1] )
	{
		output << "D"
			<< bcd1( b[1] )
			<< bcd2( b[0] ) << bcd1( b[0] )
			<< (b[1] & digital_tone_polarity_bit ? 'I' : 'N')
			;
		return;
	}

	char c = bcd2( b[1] );
	if( '0' != c )
		output << c;

	output
		<< bcd1( b[1] )
		<< bcd2( b[0] ) << '.' << bcd1( b[0] );
}
