#include "common.h"

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
	if( !input )
		return input;
	if( input >>= s )
		return input;

	std::cerr << "Wrong read! Wait word: \"" << s << '"';
        input.setstate( std::ios_base::failbit );
	return input;
}

//----------------------------------------------------------------
std::istream& operator>>( std::istream& input, const char c )
{
	if( !input )
		return input;
	if( input.peek() == c )
	{
		input.ignore();
		return input;
	}
	std::cerr << "Wrong read! Wait char: \"" << c << '"';
        input.setstate( std::ios_base::failbit );
	return input;
}
