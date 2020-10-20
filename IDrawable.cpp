#include "IDrawable.hpp"

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
