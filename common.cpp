#include "common.h"

char dec_separator = '.'; // символ-разделитель целой и дробной частей

//----------------------------------------------------------------
const char* filename = NULL;

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

	cerr << filename << ": Wrong read! Wait word: \"" << s << '"';
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
	cerr << filename << ": Wrong read! Wait char: \"" << c << '"';
	streamerror( input );
	return input;
}
