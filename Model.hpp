#pragma once
#include <string>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

#include "IDrawable.hpp"

class Model : public IDrawable
{
	Model() = delete;
public:
	Model(ID3D11Device* device, std::string file);
	~Model();
};
