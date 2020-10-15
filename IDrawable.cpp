#include "IDrawable.hpp"

XMMATRIX IDrawable::GetWorldMatrix()
{
   return worldmatrix;
}

BOOL IDrawable::IsClockwise()
{
    return clockwise;
}
