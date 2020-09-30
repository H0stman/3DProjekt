#include "Texture.hpp"

bool Texture::Initialize(int texWidth, int texHeight) {
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

	Engine *engine = Engine::GetInstance();
	HRESULT hr = engine->GetDevice()->CreateTexture2D(&texDesc, NULL, &texture);
	if (FAILED(hr)) {
		return false;
	}

	D3D11_RENDER_TARGET_VIEW_DESC rtvDesc;
	rtvDesc.Format = texDesc.Format;
	rtvDesc.ViewDimension = D3D11_RTV_DIMENSION_TEXTURE2D;
	rtvDesc.Texture2D.MipSlice = 0;

	hr = engine->GetDevice()->CreateRenderTargetView(texture.Get(), &rtvDesc, &rendertargetview);
	if (FAILED(hr)) {
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = texDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = engine->GetDevice()->CreateShaderResourceView(texture.Get(), &srvDesc, &shaderresourceview);
	if (FAILED(hr)) {
		return false;
	}

	D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc;
	uavDesc.Format = texDesc.Format;
	uavDesc.ViewDimension = D3D11_UAV_DIMENSION_TEXTURE2D;
	uavDesc.Texture2D.MipSlice = rtvDesc.Texture2D.MipSlice;

	hr = engine->GetDevice()->CreateUnorderedAccessView(texture.Get(), &uavDesc, &unorderedaccessview);
	if (FAILED(hr)) {
		return false;
	}

	return true;
}

ID3D11Texture2D* Texture::GetTexture() {
	return texture.Get();
}

ID3D11RenderTargetView* Texture::GetRenderTargetView() {
	return rendertargetview.Get();
}

ID3D11ShaderResourceView* Texture::GetShaderResourceView() {
	return shaderresourceview.Get();
}

ID3D11UnorderedAccessView* Texture::GetUnorderedAccessView() {
	return unorderedaccessview.Get();
}

bool Texture::LoadTexture(std::wstring file)
{
	Engine *engine = Engine::GetInstance();
	/*****Create Texture from file*****/
	HRESULT HR = CreateWICTextureFromFile(engine->GetDevice(), file.c_str(), textureResource.GetAddressOf(), shaderresourceview.GetAddressOf());
	if (FAILED(HR))
	{
		MessageBox(nullptr, L"Error creating texture from file.", L"ERROR", MB_OK);
		return false;
	}

	return true;
}

void Texture::Clear(float red, float green, float blue, float alpha) {
	Engine *engine = Engine::GetInstance();
	float color[4];
	color[0] = red;
	color[1] = green;
	color[2] = blue;
	color[3] = alpha;
	engine->GetContext()->ClearRenderTargetView(rendertargetview.Get(), color);
}

