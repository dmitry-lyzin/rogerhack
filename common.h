#pragma once
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <locale.h>
#include <fcntl.h>
#include <assert.h>
#include "binary.h"

using std::cin;
using std::cout;
using std::cerr;

#define fatal(x) do { perror( x); exit( EXIT_FAILURE); } while (0)

//----------------------------------------------------------------
#define SAMPLE_CLASS( cls)											\
	void read( std::istream& input );									\
	void print( std::ostream& output ) const;								\
};														\
inline std::istream& operator>>( std::istream& input,	cls& s )	{ s.read( input ); return input; }	\
inline std::ostream& operator<<( std::ostream& output,	const cls& s )	{ s.print( output ); return output; }


//----------------------------------------------------------------
inline char bcd1( uint8_t x )	{ return (x & b00001111) + '0';	}
inline char bcd2( uint8_t x )	{ return (x >> 4) + '0'; 	}


//----------------------------------------------------------------
#ifdef __GNUC__
#define NO_RETURN __attribute__((noreturn))
#elif __MINGW32__
#define NO_RETURN __attribute__((noreturn))
#elif __clang__
#define NO_RETURN __attribute__((noreturn))
#elif _MSC_VER
#define NO_RETURN __declspec(noreturn)
#endif

NO_RETURN void streamerror( std::istream& input );


//----------------------------------------------------------------
uint8_t getdigit( std::istream& input );

//----------------------------------------------------------------
inline bool operator>>=( std::istream& input, const char c )
{
	if( input.peek() != c )
		return false;

	input.ignore();
	return true;
}

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, const char c )
{
	if( input.peek() != c )
		streamerror( input );

	input.ignore();
	return input;
}

//----------------------------------------------------------------
bool operator>>=( std::istream& input, const char* s );

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, const char* s )
{
	if( input >>= s )
		return input;

	streamerror( input );
	return input;
}

//================================================================
static class Any_line {} any_line;

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Any_line s )
{
	input.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, Any_line s )
{
	output << '\n';
	return output;
}

//================================================================
static class Any_word {} any_word;

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Any_word s )
{
	char c;
	while( input.get( c))
	{
		if( !isgraph( c ) )
			break;
	}
	input.unget();
	return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, Any_word s )
{
	output << "X~X~X";
	return output;
}
