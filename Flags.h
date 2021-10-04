#pragma once
#include "common.h"

//----------------------------------------------------------------
class Flags
{
	uint8_t byte;

public:
	void read( std::istream& input );
	void print( std::ostream& output ) const;
};

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Flags& s )
{
	s.read( input ); return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, const Flags s )
{
	s.print( output );	return output;
}
