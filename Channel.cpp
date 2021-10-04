#include "Channel.h"

//----------------------------------------------------------------
void Channel::read( std::istream& input )
{
	if( input.eof() )
		return;

	input	>> receive >> '\t' >> transmit >> '\t'
		>> rx_tone >> '\t' >> tx_tone >> '\t'
		>> flags;
}

//----------------------------------------------------------------
void Channel::print( std::ostream& output ) const
{
	output	<< receive << '\t' << transmit << '\t'
		<< rx_tone << '\t' << tx_tone << '\t'
		<< flags;
}
