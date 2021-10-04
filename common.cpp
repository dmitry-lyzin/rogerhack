#include "common.h"

//----------------------------------------------------------------
//[[noreturn]] void streamerror( std::istream & input)
NO_RETURN void streamerror( std::istream& input )
{
	cerr << "wrong read!\n===================>";
	while( !cin.eof() )
	{
		char c;
		cin.get( c );
		cerr.put( c );
	}
	cerr << "<===================\n";
	exit( EXIT_FAILURE );
}

//----------------------------------------------------------------
uint8_t getdigit( std::istream& input )
{
	uint8_t c = input.get();
	if( isdigit( c ) )
		return c - '0';

	input.putback( c );
	streamerror( input );
}

//----------------------------------------------------------------
bool can_to_get( std::istream& input, const char* s )
{
	const char* p = s;
	while( *p )
	{
		if( input.peek() != *p++ )
		{
			--p;
			while( p > s )
				input.putback( *--p );
			return false;
		}
		input.ignore();
	}
	return true;
}
