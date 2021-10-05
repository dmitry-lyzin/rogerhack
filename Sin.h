#pragma once
#include "common.h"
#include "Channel.h"

//----------------------------------------------------------------
class Sin
{
	union
	{
		char buf[2048];
		struct {
			uint8_t head[16];
			Channel channels[80];
		} s;
	} u;

public:
	void load( const char* filename );
	void save( const char* filename ) const;
	SAMPLE_CLASS
( Sin )
