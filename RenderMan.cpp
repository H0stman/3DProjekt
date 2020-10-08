#include "RenderMan.hpp"

RenderMan::RenderMan()
{
	geometry.push_back(new Terrain(""));
	renderer.push_back(new VanillaRenderer());
}

RenderMan::~RenderMan()
{
	for (IDrawable *g : geometry)
		delete g;
	for (IRenderer *r : renderer)
		delete r;
}
