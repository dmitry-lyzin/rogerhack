#include "common.h"

//----------------------------------------------------------------
//[[noreturn]] void streamerror( std::istream & input)
NO_RETURN void streamerror( std::istream& input )
{
	input.clear();
	cerr << "\n===================>";
	int c = input.get();
	while( c > 0 )
	{
		cerr.put( c );
		c = input.get();
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

	input.unget();
	streamerror( input );
}

//----------------------------------------------------------------
bool operator>>=( std::istream& input, const char* s )
{
	const char* p = s;
	while( *p )
	{
		if( input.peek() != *p++ )
		{
			--p;
			for( size_t i = p - s; i > 0; --i )
				input.unget();
			return false;
		}
		input.ignore();
	}
	return true;
}

//----------------------------------------------------------------
std::istream& operator>>( std::istream& input, const char* s )
{
	if( input >>= s )
		return input;

	cerr << "Wrong read! Wait word: \"" << s << '"';
	streamerror( input );
	return input;
}

//----------------------------------------------------------------
std::istream& operator>>( std::istream& input, const char c )
{
	if( input.peek() == c )
	{
		input.ignore();
		return input;
	}
	cerr << "Wrong read! Wait char: \"" << c << '"';
	streamerror( input );
	return input;
}
