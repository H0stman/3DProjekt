#pragma once
#include "Camera.hpp"
#include "Terrain.hpp"
#include "VanillaRenderer.hpp"

class IDrawable;
class IRenderer;

class RenderMan
{
	Camera camera;
	std::vector<IDrawable*> geometry;
	std::vector<IRenderer*> renderer;

public:
	RenderMan();
	~RenderMan();

	VOID Update();
};
