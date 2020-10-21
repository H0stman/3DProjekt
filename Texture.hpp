#pragma once
#include <string>
#include <DirectXTK/WICTextureLoader.h>
#include <atlbase.h>

using namespace DirectX;

class Texture
{
	ID3D11Texture2D *texture;
	ID3D11Resource *textureResource;
	ID3D11RenderTargetView *rendertargetview;
	ID3D11ShaderResourceView *shaderresourceview;
	ID3D11UnorderedAccessView *unorderedaccessview;

public:
	Texture(INT texWidth, INT texHeight, ID3D11Device *device);
	Texture(std::string file, ID3D11Device *device);
	~Texture();

	ID3D11Texture2D* GetTexture();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11UnorderedAccessView* GetUnorderedAccessView();
	BOOL LoadTexture(std::string file, ID3D11Device* device);

	VOID Clear(FLOAT red, FLOAT green, FLOAT blue, FLOAT alpha, ID3D11DeviceContext* context);
};

