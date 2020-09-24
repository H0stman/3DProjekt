#pragma once
#include "Engine.hpp"

class Texture
{
	Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> rendertargetview;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> shaderresourceview;
	Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> unorderedaccessview;

public:
	Texture() = default;
	~Texture() = default;
	bool Initialize(ID3D11Device * device, int texWidth, int texHeight);

	ID3D11Texture2D* GetTexture();
	ID3D11RenderTargetView* GetRenderTargetView();
	ID3D11ShaderResourceView* GetShaderResourceView();
	ID3D11UnorderedAccessView* GetUnorderedAccessView();
	bool LoadTexture(ID3D11Device* device, std::string file);

	void Clear(ID3D11DeviceContext * context, float red, float green, float blue, float alpha);
};

