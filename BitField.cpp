#include "BitField.h"

//----------------------------------------------------------------
void BitField::read( std::istream& input )
{
	for( size_t i = 0; i < state_names_len; i++ )
	{
		if( input >>= state_names[i] )
		{
			register auto r = *operand.pointer;
			r &= ~bitmask;
			r |= i << bitindex;
			*operand.pointer = r;
			return;
		}
	}
}
