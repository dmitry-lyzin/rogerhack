#pragma once
#include "common.h"
#include "BCDnum.h"

//----------------------------------------------------------------
class Tone
{
	union
	{
		uint16_t bin;
		BCDnum< uint16_t, 0x0, 4, '.', 1, '0'> analog_tone;
		BCDnum< uint16_t, '0', 3, '.', 0, '0'> digital_tone;
	} u;
public:
	SAMPLE_CLASS
( Tone )