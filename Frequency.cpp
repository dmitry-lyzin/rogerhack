#include "Frequency.h"

//----------------------------------------------------------------
void Frequency::read( std::istream& input )
{
	if( input >>= '-' )
	{
		input >> any_word;
		b = 0xffffffffu;
		return;
	}
	input >> b;
}

//----------------------------------------------------------------
void Frequency::print( std::ostream& output ) const
{
	const auto flag = 0xff000000u & b;
	if( flag == 0 || flag == 0xff000000u )
	{
		output << "---------";
		return;
	}
	output << b; 
}
