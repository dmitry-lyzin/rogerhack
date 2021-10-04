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
bool can_to_get( std::istream& input, const char* s );

//----------------------------------------------------------------
inline bool can_to_get( std::istream& input, const char c )
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
inline std::istream& operator>>( std::istream& input, const char* s )
{
	if( can_to_get( input, s ) )
		return input;

	streamerror( input );
	return input;
}

//================================================================
static class Str_tail {} str_tail;

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Str_tail s )
{
	input.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, Str_tail s )
{
	output << '\n';
	return output;
}

//================================================================
static class Someword {} someword;

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Someword s )
{
	char c;
	while( input.get( c))
	{
		if( !isgraph( c ) )
			break;
	}
	input.putback( c );
	return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, Someword s )
{
	output << "X~X~X";
	return output;
}
