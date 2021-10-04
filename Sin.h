#pragma once
#include "common.h"
#include "Channel.h"

//----------------------------------------------------------------
class Sin
{
	union
	{
		char buf[2048];
		struct {
			uint8_t head[16];
			Channel channels[80];
		} s;
	} u;

public:
	void load( const char* filename );
	void read( std::istream& input );
	void print( std::ostream& output ) const;
};

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Sin& s )
{
	s.read( input ); return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, const Sin& s )
{
	s.print( output );	return output;
}
