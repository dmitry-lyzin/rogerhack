#pragma once
#include "common.h"
#include "BitField.h"
#include "BCDnum.h"

#define TPS(x) const char, (x), std::size(x)

static const char polarity_bit_str[] = { 'N', 'I' };

//----------------------------------------------------------------
class Tone
{
	union
	{
		uint16_t bin;
		BitField< uint16_t, TPS( polarity_bit_str ), 14>	polarity_bit;
		BCDnum	< uint16_t, 0x0, 4, '.', 1, '0'>		analog_tone;
		BCDnum	< uint16_t, '0', 3, '.', 0, '0'>		digital_tone;
	} u;
public:
	SAMPLE_CLASS
( Tone )

#undef TPS