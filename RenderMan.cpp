#include "RenderMan.hpp"

RenderMan::RenderMan()
{
	geometry.push_back(new Terrain("heightmap.bmp"));
	renderer.push_back(new VanillaRenderer());
}

RenderMan::~RenderMan()
{
	for (IDrawable *g : geometry)
		delete g;
	for (IRenderer *r : renderer)
		delete r;
}

VOID RenderMan::Update()
{
	geometry[0]->PrimePipeline(VANILLA);
	renderer[0]->Draw(1u, &geometry[0], 0u, nullptr);
}
