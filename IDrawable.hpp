#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <vector>

#include "Texture.hpp"

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
	std::vector<vertex> vertices;
	std::vector<DWORD> indices;
	ID3D11Buffer* vertexbuffer, *indexbuffer;

	enum textype {diffuse, displacement, normalmap};
	Texture *texture[3];

public:
	virtual ~IDrawable() {};
	virtual UINT GetIndexCount() = 0;
	virtual UINT GetStartIndexLocation() = 0;
	virtual INT GetBaseVertexLocation() = 0;
	virtual ID3D11Buffer** GetVertexBuffer() = 0;
	virtual ID3D11Buffer* GetIndexBuffer() = 0;
	Texture* GetTextures();
	XMMATRIX GetWorldMatrix();
	BOOL IsClockwise();
	BoundingBox GetBoundingBox();
};

