#include "Flags.h"
#include "BitField.h"

//----------------------------------------------------------------
const char* busy2str [] = { "OFF",	"QT/DQT", "Carrier", NULL };
const char* scan2str [] = { "Delete",	"Add"	};
const char* power2str[] = { "Low",	"High"	};

BitField< uint8_t, const char*> busy ( PS( busy2str  ), 0 );
BitField< uint8_t, const char*> scan ( PS( scan2str  ), 3 );
BitField< uint8_t, const char*> power( PS( power2str ), 4 );

//----------------------------------------------------------------
void Flags::read( std::istream& input )
{
	input	>> busy ( &value ) >> '\t'
		>> scan ( &value ) >> '\t'
		>> power( &value );
}

//----------------------------------------------------------------
void Flags::print( std::ostream& output ) const
{
	output	<< busy ( value ) << '\t'
		<< scan ( value ) << '\t'
		<< power( value );
}
