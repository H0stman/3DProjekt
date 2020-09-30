#pragma once
#include "Engine.hpp"
#include <string>
#include <DirectXTK/WICTextureLoader.h>

class Texture
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11Resource> textureResource;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rendertargetview;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderresourceview;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> unorderedaccessview;

public:
	Texture() = default;
	~Texture() = default;
	bool Initialize(int texWidth, int texHeight);

	ID3D11Texture2D* GetTexture();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11UnorderedAccessView* GetUnorderedAccessView();
	bool LoadTexture(std::wstring file);

	void Clear(float red, float green, float blue, float alpha);
};

