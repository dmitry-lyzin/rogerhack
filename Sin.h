#pragma once
#include "common.h"
#include "Channel.h"

//----------------------------------------------------------------
class Sin
{
public:
	union
	{
		uint8_t buf[2048];
		struct {
			uint8_t head[16];
			Channel channels[80];
		} s;
	} u;
	void load( const char* filename );
	void save( const char* filename ) const;
	SAMPLE_CLASS
( Sin )

//----------------------------------------------------------------
extern Sin SIN;
