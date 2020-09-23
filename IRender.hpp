#pragma once
#include "Engine.hpp"

class IRender
{
public:
	virtual void Draw(UINT uIndexCount, UINT uStartIndexLocation, INT baseVertexLocation) = 0;
};

