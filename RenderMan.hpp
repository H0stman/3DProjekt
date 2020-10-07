#pragma once
#include "Camera.hpp"
#include "IDrawable.hpp"
#include "IRenderer.hpp"

class RenderMan
{
	Camera camera;
	IDrawable* geometry;
	IRenderer* renderer;

public:
	VOID Update();
};
