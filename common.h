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
inline
bool operator>>=( std::istream& input, const char c )
{
	if( input.peek() != c )
		return false;

	input.ignore();
	return true;
}
bool operator>>=( std::istream& input, const char* s );

//----------------------------------------------------------------
std::istream& operator>>( std::istream& input, const char c );
std::istream& operator>>( std::istream& input, const char* s );

//================================================================
static class Any_line {} any_line;
inline std::istream& operator>>( std::istream& input, Any_line s )
{
	input.ignore( std::numeric_limits<std::streamsize>::max(), '\n' );
	return input;
}
inline std::ostream& operator<<( std::ostream& output, Any_line s )
{
	output << '\n';
	return output;
}

//================================================================
static class Any_word {} any_word;
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
inline std::ostream& operator<<( std::ostream& output, Any_word s )
{
	output << 'X';
	return output;
}

//================================================================
class Digit_char
{
	char c;
public:
	Digit_char(			) : c( '\0'	) {};
	Digit_char( const char c_	) : c(  c_-'0'	) {};

	operator char() const { return c; };

	bool mayread( std::istream& input )
	{
		int c0 = input.peek();
		if( !isdigit( c0 ) )
			return false;

		c = c0 - '0';
		input.ignore();
		return true;
	};

	void read( std::istream& input )
	{
		int c0 = input.peek();
		if( isdigit( c0 ) )
		{
			c = c0 - '0';
			input.ignore();
			return;
		}
		cerr << "Wrong read! Char: \"" << char( c0 ) << "\" not is digit";
		streamerror( input );
	};
	void print( std::ostream& output ) const { output << (c + '0'); };
};

//----------------------------------------------------------------
inline bool		operator>>=( std::istream& input,	Digit_char &c) { return c.mayread( input );		}
inline std::istream&	operator>> ( std::istream& input,	Digit_char &c) { c.read( input );	return input;	}
inline std::ostream&	operator<< ( std::ostream& output, const Digit_char c) { c.print( output );	return output;	}
