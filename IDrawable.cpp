#include "IDrawable.hpp"

std::vector<Texture*> IDrawable::GetTextures()
{
   return texture;
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

VOID IDrawable::Transform(FXMMATRIX trans)
{
	worldmatrix *= trans;
	boundingbox.Transform(boundingbox, trans);
}

std::string IDrawable::GetName()
{
	return name;
}
