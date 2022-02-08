#pragma once
#include "common.h"
#include "BitField.h"

#define TPS(x) const char*, (x), (sizeof(x)/sizeof(*(x)))

static const char* busy2str [] = { "OFF",	"QT/DQT", "Carrier",	NULL	};
static const char* scan2str [] = { "Delete",	"Add"	};
static const char* power2str[] = { "Low",	"High"	};

class Flags
{
	union
	{
		BitField< uint8_t, 0, TPS( busy2str )> busy;
		BitField< uint8_t, 3, TPS( scan2str )> scan;
		BitField< uint8_t, 4, TPS( power2str)> power;
	} u;
public:
	SAMPLE_CLASS
( Flags )

#undef TPS