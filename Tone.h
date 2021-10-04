#pragma once
#include "common.h"

//----------------------------------------------------------------
class Tone
{
	uint8_t b[2];
public:
	void read( std::istream& input );
	void print( std::ostream& output ) const;
};

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Tone& s )
{
	s.read( input ); return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, const Tone s )
{
	s.print( output );	return output;
}
