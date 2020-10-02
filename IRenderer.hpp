#pragma once
#include "Engine.hpp"

class IRenderer
{
public:
	virtual ~IRenderer();
	virtual void Draw(UINT numDrawables, IDrawable** ppDrawables, UINT numTargets, Texture **ppRenderTargets) = 0;
};

