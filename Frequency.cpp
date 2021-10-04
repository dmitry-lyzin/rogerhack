#include "Frequency.h"

//----------------------------------------------------------------
void Frequency::read( std::istream& input )
{
	if( input.peek() == '-' )
	{
		input >> someword;
		memset( b, 0xFFu, std::size( b ) );
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
			input.putback( c );
			break;
		}
		if( p < buf + std::size( buf ) )
			*p++ = c - '0';
	}

	if( ! can_to_get( input, '.' ))
		streamerror( input );

	memset( this, 0, sizeof( *this ) );
	b[2] = (*--p) << 4;
	b[3] = (*--p);
	b[3] |= (*--p) << 4;
	memset( buf, 0, std::size( buf ) );
	p = buf;
	while( input.get( c ) )
	{
		if( !isdigit( c ) )
		{
			input.putback( c );
			break;
		}

		if( p >= buf + std::size( buf ) )
			break;

		*p++ = c - '0';
	}
	p = buf;
	b[2] |= *p++;
	b[1] = (*p++) << 4;
	b[1] |= *p++;
	b[0] = (*p++) << 4;
	b[0] |= *p++;
}

//----------------------------------------------------------------
void Frequency::print( std::ostream& output ) const
{
	if( b[3] == 0xFFu )
	{
		output << "---------";
		return;
	}

	char c = bcd2( b[3] );
	if( '0' != c )
		output << c;

	output
		<< bcd1( b[3] )
		<< bcd2( b[2] )
		<< '.'
		<< bcd1( b[2] )
		<< bcd2( b[1] ) << bcd1( b[1] )
		<< bcd2( b[0] ) << bcd1( b[0] )
		;
}
