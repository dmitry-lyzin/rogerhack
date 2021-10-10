#pragma once
#include "common.h"
#include "BCDnum.h"

//----------------------------------------------------------------
class Frequency
{
	BCDnum< uint32_t, 5, false> b;
public:
	SAMPLE_CLASS
( Frequency )