#pragma once
#include "IDrawable.hpp"
#include <vector>

class Terrain : public IDrawable
{
	struct HeightMapInfo //Heightmap structure
	{
		UINT terrainWidth;			//Width of heightmap
		UINT terrainHeight;			//Height (Length) of heightmap
		XMFLOAT3* heightMap;			//Array to store terrain's vertex positions
	};

	std::vector<vertex> vertices;
	std::vector<DWORD> indices;
	HeightMapInfo hminfo;

	UINT facecount, vertexcount, stride, offset;

	ID3D11Buffer *indexbuffer, *vertexbuffer;

public:
	Terrain(PCSTR filename, ID3D11Device* device);
	virtual ~Terrain();
	

	virtual UINT GetIndexCount();
	virtual UINT GetStartIndexLocation();
	virtual INT GetBaseVertexLocation();
};

