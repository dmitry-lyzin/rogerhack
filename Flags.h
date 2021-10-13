#pragma once
#include "common.h"
#include "BitField.h"

#define TPS(x) const char*, (x), std::size(x)

static const char* busy2str [] = { "OFF",	"QT/DQT", "Carrier",	NULL	};
static const char* scan2str [] = { "Delete",	"Add"	};
static const char* power2str[] = { "Low",	"High"	};

class Flags
{
	union
	{
		BitField< uint8_t, TPS( busy2str ), 0> busy;
		BitField< uint8_t, TPS( scan2str ), 3> scan;
		BitField< uint8_t, TPS( power2str), 4> power;
	} u;
public:
	SAMPLE_CLASS
( Flags )

#undef TPS