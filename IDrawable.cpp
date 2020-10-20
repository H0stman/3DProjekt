#include "IDrawable.hpp"

Texture* IDrawable::GetTextures()
{
   return texture[0];
}

IDrawable::~IDrawable() {
	if(vertexbuffer != nullptr) vertexbuffer->Release();
	if(indexbuffer != nullptr) indexbuffer->Release();
	for (size_t i = 0; i < 2; ++i) {
		if (texture[i] != nullptr) delete texture[i];
	}
}

XMMATRIX IDrawable::GetWorldMatrix()
{
   return worldmatrix;
}

BOOL IDrawable::IsClockwise()
{
    return clockwise;
}

BoundingBox IDrawable::GetBoundingBox()
{
	return boundingbox;
}
