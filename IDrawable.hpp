#pragma once
#include "Engine.hpp"

struct vertex
{
	XMFLOAT3 position;
	XMFLOAT2 texturecoordinate;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
};

enum RenderType { VANILLA = 1, DEFERRED = 2, SHADOWMAP = 4, BLUR = 8 };

class IDrawable
{
protected:
	

public:
	virtual ~IDrawable() {};
	virtual void PrimePipeline(UINT pipelinesettings) = 0;
	virtual UINT GetIndexCount() = 0;
	virtual UINT GetStartIndexLocation() = 0;
	virtual INT GetBaseVertexLocation() = 0;
};

