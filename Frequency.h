#pragma once
#include "common.h"

//----------------------------------------------------------------
class Frequency
{
	uint8_t b[4];
public:
	void read	( std::istream& input );
	void print	( std::ostream& output ) const;
};

//----------------------------------------------------------------
inline std::istream& operator>>( std::istream& input, Frequency& s )
{
	s.read( input );
	return input;
}

//----------------------------------------------------------------
inline std::ostream& operator<<( std::ostream& output, const Frequency s )
{
	s.print( output );
	return output;
}
