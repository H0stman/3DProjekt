#pragma once
#include "Engine.hpp"

class IDrawable
{

public:
	virtual void PrimePipeline(UINT pipelinesettings) = 0;
};

