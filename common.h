#pragma once
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <locale.h>
#include <fcntl.h>
#include <assert.h>

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

extern char dec_separator;

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

extern const char* filename;
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
public:
	char value;

	Digit_char(			) : value(  '\0') {};
	Digit_char( const char c	) : value( c-'0') {};

	bool mayread( std::istream& input )
	{
		int c = input.peek() - '0';
		if( c < 0 || c > 9 )
			return false;

		value = c;
		input.ignore();
		return true;
	};

	void read( std::istream& input )
	{
		int c = input.peek() - '0';
		if( 0 <= c && c <= 9 )
		{
			value = c;
			input.ignore();
			return;
		}
		cerr << filename << ": Wrong read! Char: \"" << char( c + '0' ) << "\" not is digit";
		streamerror( input );
	};
	void print( std::ostream& output ) const { output << (value + '0'); };
};

//----------------------------------------------------------------
inline bool		operator>>=( std::istream& input,	Digit_char &value) { return value.mayread( input );		}
inline std::istream&	operator>> ( std::istream& input,	Digit_char &value) { value.read( input );	return input;	}
inline std::ostream&	operator<< ( std::ostream& output, const Digit_char value) { value.print( output );	return output;	}
