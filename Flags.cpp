#include "Flags.h"
#include "BitField.h"

//----------------------------------------------------------------
const char* busy2str [] = { "OFF",	"QT/DQT", "Carrier", "?" };
const char* scan2str [] = { "Delete",	"Add"	};
const char* power2str[] = { "Low",	"High"	};

#define PS(x) x, std::size(x)

BitField busy ( PS( busy2str  ), 0 );
BitField scan ( PS( scan2str  ), 3 );
BitField power( PS( power2str ), 4 );

//----------------------------------------------------------------
void Flags::read( std::istream& input )
{
	input	>> busy ( &byte ) >> '\t'
		>> scan ( &byte ) >> '\t'
		>> power( &byte );
}

//----------------------------------------------------------------
void Flags::print( std::ostream& output ) const
{
	output	<< busy ( byte ) << '\t'
		<< scan ( byte ) << '\t'
		<< power( byte );
}
