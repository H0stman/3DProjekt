#include "IDrawable.hpp"

BOOL IDrawable::IsClockwise()
{
    return clockwise;
}
#include "IDrawable.hpp"

BoundingBox IDrawable::GetBoundingBox()
{
	return boundingbox;
}
