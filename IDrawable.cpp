#include "IDrawable.hpp"

XMMATRIX IDrawable::GetWorldMatrix()
{
   return worldmatrix;
}

BOOL IDrawable::IsClockwise()
{
    return clockwise;
}
#include "IDrawable.hpp"

BoundingBox IDrawable::GetBoundingBox()
{
	return boundingbox;
}
