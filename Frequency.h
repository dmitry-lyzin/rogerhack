#pragma once
#include "common.h"
#include "BCDnum.h"

//----------------------------------------------------------------
class Frequency : public BCDnum< uint32_t, 0x0, 8, 5>
{
	typedef BCDnum< uint32_t, 0x0, 8, 5> Super;

public:
	SAMPLE_CLASS
( Frequency )
