#pragma once
#include "common.h"
#include "Frequency.h"
#include "Tone.h"
#include "Flags.h"

//----------------------------------------------------------------
class Channel
{
	Frequency receive;
	Frequency transmit;
	Tone rx_tone;
	Tone tx_tone;
	Flags flags;
	uint8_t x01;	// хз что, там лежит 0x01
	uint8_t x01_;	// хз что, там лежит 0x01
	uint8_t xFF;	// хз что, там лежит 0xFF

public:
	SAMPLE_CLASS
( Channel )