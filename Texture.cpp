#include "Texture.hpp"
#include <comdef.h>

Texture::Texture(INT texWidth, INT texHeight, ID3D11Device* device) : rendertargetview(nullptr), shaderresourceview(nullptr), textureResource(nullptr), texture(nullptr), unorderedaccessview(nullptr)
{
	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));
	texDesc.Width = texWidth;
	texDesc.Height = texHeight;
	texDesc.MipLevels = 1;
	texDesc.ArraySize = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.SampleDesc.Count = 1;
	texDesc.Usage = D3D11_USAGE_DEFAULT;
	texDesc.BindFlags = D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE | D3D11_BIND_UNORDERED_ACCESS;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;

	HRESULT hr = device->CreateTexture2D(&texDesc, nullptr, &texture);

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = device->CreateRenderTargetView(texture, &rtvDesc, &rendertargetview);

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = device->CreateShaderResourceView(texture, &srvDesc, &shaderresourceview);

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = texDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = rtvDesc.Texture2D.MipSlice;

	hr = device->CreateUnorderedAccessView(texture, &uavDesc, &unorderedaccessview);
}

Texture::Texture(std::string file, ID3D11Device* device) : rendertargetview(nullptr), shaderresourceview(nullptr), textureResource(nullptr), texture(nullptr), unorderedaccessview(nullptr)
{
	LoadTexture(file, device);	
}

Texture::~Texture()
{
	if(texture != nullptr) texture->Release();
	if(textureResource != nullptr) textureResource->Release();
	if(rendertargetview != nullptr) rendertargetview->Release();
	if(shaderresourceview != nullptr) shaderresourceview->Release();
	if(unorderedaccessview != nullptr) unorderedaccessview->Release();
}

ID3D11Texture2D* Texture::GetTexture()
{
	return texture;
}

ID3D11RenderTargetView* Texture::GetRenderTargetView() 
{
	return rendertargetview;
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() 
{
	return shaderresourceview;
}

ID3D11UnorderedAccessView* Texture::GetUnorderedAccessView() 
{
	return unorderedaccessview;
}

BOOL Texture::LoadTexture(std::string file, ID3D11Device* device)
{
	//Create Texture from file
	HRESULT hr = CreateWICTextureFromFile(device, CA2W(file.c_str()), &textureResource, &shaderresourceview);

	return TRUE;
}

VOID Texture::Clear(const FLOAT* clearColour, ID3D11DeviceContext* context) 
{
	context->ClearRenderTargetView(rendertargetview, clearColour);
}

