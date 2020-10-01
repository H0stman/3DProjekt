#pragma once
#include "IRenderer.hpp"
#include "PixelShader.hpp"

class VanillaRenderer : IRenderer {
	PixelShader pixelshader;

public:
	VanillaRenderer();
	~VanillaRenderer();

	void Draw(IDrawable **ppDrawables, UINT numDrawables);
};
