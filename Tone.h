#pragma once
#include "common.h"
#include "BCDnum.h"

static constexpr const char  analog_tone_fmt[] = "-4.10";
static constexpr const char digital_tone_fmt[] = "03.00";

//----------------------------------------------------------------
class Tone
{
	union
	{
		uint16_t bin;
		BCDnum< uint16_t,  analog_tone_fmt> analog_tone;
		BCDnum< uint16_t, digital_tone_fmt> digital_tone;
	} u;
public:
	SAMPLE_CLASS
( Tone )