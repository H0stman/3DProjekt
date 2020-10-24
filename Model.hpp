#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


#include "IDrawable.hpp"

class Model : public IDrawable
{
	Model() = delete;

protected:


public:
	Model(std::string file, ID3D11Device* device);
	Model(std::string file, XMMATRIX translate, ID3D11Device* device);
	~Model();
	UINT GetIndexCount();
	UINT GetStartIndexLocation();
	INT GetBaseVertexLocation();
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
};
