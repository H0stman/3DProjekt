#pragma once
#include "IDrawable.hpp"

class Terrain : private IDrawable
{
	struct HeightMapInfo // Heightmap structure
	{
		INT terrainWidth;				// Width of heightmap
		INT terrainHeight;			// Height (Length) of heightmap
		XMFLOAT3* heightMap;			// Array to store terrain's vertex positions
	};

	std::vector<vertex> vertices;
	std::vector<DWORD> indices;
	HeightMapInfo hminfo;
	UINT facecount, vertexcount;

	ID3D11Buffer* indexbuffer;
	ID3D11Buffer* vertexbuffer;
	

	void ReadFileData(PCSTR filename);
	void CreateVerticesAndIndexList();

public:
	Terrain(PCSTR filename);
	~Terrain();

	void PrimePipeline(UINT pipelinesettings) override;
};

