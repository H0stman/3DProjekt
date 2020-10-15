#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>

using namespace DirectX;

struct vertex
{
	XMFLOAT3 position;
	XMFLOAT2 texturecoordinate;
	XMFLOAT3 normal;
	XMFLOAT3 tangent;
	XMFLOAT3 bitangent;
};

class IDrawable
{	
protected:
	BOOL clockwise;
	XMMATRIX worldmatrix;
	BoundingBox boundingbox;

public:
	virtual ~IDrawable() {};
	virtual UINT GetIndexCount() = 0;
	virtual UINT GetStartIndexLocation() = 0;
	virtual INT GetBaseVertexLocation() = 0;
	virtual ID3D11Buffer** GetVertexBuffer() = 0;
	virtual ID3D11Buffer* GetIndexBuffer() = 0;
	BOOL IsClockwise();
	BoundingBox GetBoundingBox();
};

