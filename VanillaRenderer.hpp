#pragma once
#include "IRenderer.hpp"
#include "PixelShader.hpp"

class VanillaRenderer : public IRenderer {
	PixelShader pixelshader;

public:
	VanillaRenderer();
	~VanillaRenderer();

	void Draw(UINT numDrawables, IDrawable** ppDrawables, UINT numTargets, Texture **ppRenderTargets);
};
