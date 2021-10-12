#pragma once
#include "common.h"
#include "BCDnum.h"

//----------------------------------------------------------------
class Frequency : public BCDnum< uint32_t, '-', 8,'.', 5,'0'>
{
	typedef BCDnum< uint32_t, '-', 8, '.', 5, '0'> Super;

public:
	SAMPLE_CLASS
( Frequency )
