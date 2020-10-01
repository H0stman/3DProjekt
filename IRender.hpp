#pragma once
#include "Engine.hpp"

class IRender
{
public:
	virtual void Draw(IDrawable **ppDrawables, UINT numDrawables) = 0;
};

