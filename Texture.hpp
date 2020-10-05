#pragma once
#include "Engine.hpp"
#include <string>
#include <DirectXTK/WICTextureLoader.h>

class Texture
{
	ID3D11Texture2D *texture;
	ID3D11Resource *textureResource;
	ID3D11RenderTargetView *rendertargetview;
	ID3D11ShaderResourceView *shaderresourceview;
	ID3D11UnorderedAccessView *unorderedaccessview;

public:
	Texture(INT texWidth, INT texHeight);
	~Texture();

	ID3D11Texture2D* GetTexture();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11UnorderedAccessView* GetUnorderedAccessView();
	BOOL LoadTexture(std::wstring file);

	VOID Clear(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha);
};

