#pragma once
#include "IDrawable.hpp"

class IRenderer
{
public:
	~IRenderer() = default;
	IRenderer() = default;
	virtual void Draw(UINT numDrawables, IDrawable** ppDrawables, UINT numTargets, Texture **ppRenderTargets) = 0;
};

