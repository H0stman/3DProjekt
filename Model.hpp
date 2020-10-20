#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


#include "IDrawable.hpp"
#include "Texture.hpp"

class Model : public IDrawable
{
	enum textype {diffuse, displacement, normalmap};
	Texture *texture[3];
	
	Model() = delete;
public:
	Model(std::string file, ID3D11Device* device);
	~Model();
	UINT GetIndexCount();
	UINT GetStartIndexLocation();
	INT GetBaseVertexLocation();
	ID3D11Buffer** GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
};
