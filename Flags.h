#pragma once
#include "common.h"
#include "BitField.h"

static const char* busy2str [] = { "OFF",	"QT/DQT", "Carrier",	NULL	};
static const char* scan2str [] = { "Delete",	"Add"	};
static const char* power2str[] = { "Low",	"High"	};
#define PS(x) x, std::size(x)

class Flags
{
	union
	{
		BitField< uint8_t, PS( busy2str ), 0> busy;
		BitField< uint8_t, PS( scan2str ), 3> scan;
		BitField< uint8_t, PS( power2str), 4> power;
	} u;
public:
	SAMPLE_CLASS
( Flags )