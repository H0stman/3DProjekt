#pragma once
#include "IDrawable.hpp"

class Terrain : IDrawable
{
	struct HeightMapInfo //Heightmap structure
	{
		INT terrainWidth;				//Width of heightmap
		INT terrainHeight;			//Height (Length) of heightmap
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
	~Terrain();

	VOID PrimePipeline(UINT pipelinesettings) override;
	UINT GetIndexCount() override;
	UINT GetStartIndexLocation() override;
	INT GetBaseVertexLoation() override;
};

