#pragma once
#include "common.h"
#include "BCDnum.h"

static constexpr const char fmt[] = "-8.50";

//----------------------------------------------------------------
class Frequency : public BCDnum< uint32_t, fmt>
{
	typedef BCDnum< uint32_t, fmt> Super;

public:
	SAMPLE_CLASS
( Frequency )
