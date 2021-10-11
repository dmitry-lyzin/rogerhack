#include "Frequency.h"

//----------------------------------------------------------------
void Frequency::read( std::istream& input )
{
	if( input >>= '-' )
	{
		input >> any_word;
		value = 0xffffffffu;
		return;
	}
	Super::read( input );
}

//----------------------------------------------------------------
void Frequency::print( std::ostream& output ) const
{
	const auto flag = 0xff000000u & value;
	//if( flag == 0 || flag == 0xff000000u )
	if( flag == 0xff000000u )
	{
		output << "---------";
		return;
	}
	Super::print( output );
}
