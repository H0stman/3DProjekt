#pragma once
#include "IDrawable.hpp"

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
	ID3D11RasterizerState* ccwcullmode;

public:
	Terrain(PCSTR filename);
	virtual ~Terrain();

	virtual VOID PrimePipeline(UINT pipelinesettings);
	virtual UINT GetIndexCount();
	virtual UINT GetStartIndexLocation();
	virtual INT GetBaseVertexLocation();
};

