#pragma once
#include "Engine.hpp"

class IDrawable
{
protected:
	struct vertex
	{
		XMFLOAT3 position;
		XMFLOAT2 texturecoordinate;
		XMFLOAT3 normal;
		XMFLOAT3 tangent;
		XMFLOAT3 bitangent;
	};

public:
	virtual ~IDrawable();
	virtual void PrimePipeline(UINT pipelinesettings) = 0;
	virtual UINT GetIndexCount() = 0;
	virtual UINT GetStartIndexLocation() = 0;
	virtual INT GetBaseVertexLoation() = 0;
};

