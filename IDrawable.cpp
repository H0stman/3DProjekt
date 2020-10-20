#include "IDrawable.hpp"

Texture* IDrawable::GetTextures()
{
   return texture[0];
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
