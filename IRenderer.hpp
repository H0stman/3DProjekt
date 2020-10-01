#pragma once
#include "Engine.hpp"

class IRenderer
{
public:
	virtual ~IRenderer();
	virtual void Draw(IDrawable **ppDrawables, UINT numDrawables) = 0;
};

