#include "Flags.h"

//----------------------------------------------------------------
void Flags::read( std::istream& input )
{
	input	>> u.busy >> '\t'
		>> u.scan >> '\t'
		>> u.power;
}

//----------------------------------------------------------------
void Flags::print( std::ostream& output ) const
{
	output	<< u.busy << '\t'
		<< u.scan << '\t'
		<< u.power;
}
