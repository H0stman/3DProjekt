#include "ShadowMap.hpp"

ShadowMap::ShadowMap()
	: m_ViewPort{}, m_Hresult{}, m_ShadowMapSize{ 0.0f }, m_ShadowBias{ 0.0f }, m_PcfCount{ 2 }
{

}

bool ShadowMap::Initialize(ID3D11Device* device, const unsigned int& width, const unsigned int& height)
{
	/*Viewport creation based on chosen shadow map texture resolution*/
	m_ViewPort.TopLeftX = 0.0f;
	m_ViewPort.TopLeftY = 0.0f;
	m_ViewPort.Width = static_cast<FLOAT>(width);
	m_ViewPort.Height = static_cast<FLOAT>(height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

	D3D11_TEXTURE2D_DESC textureDescriptor;
	ZeroMemory(&textureDescriptor, sizeof(D3D11_TEXTURE2D_DESC));

	/*The resolution of the texture will affect the overall quality of the generated shadows and can therefore be set according
	  to personal preference via Initialize-method. A higher texture resolution will obviously adversely affect performance.*/
	textureDescriptor.Width = width;
	textureDescriptor.Height = height;
	textureDescriptor.MipLevels = 1u;														//Maximum number of mipmap levels.
	textureDescriptor.ArraySize = 1u;														//One texture.

	/*Because of shenanigans (cast compatibility?) the depth stencil view will interpret the format of the data as DXGI_FORMAT_D24_UNORM_S8_UINT
	 and the shader resource view will interpret the data as DXGI_FORMAT_R24_UNORM_X8_TYPELESS, hence why a typeless
	 texture format is chosen to supply support for both*/
	textureDescriptor.Format = DXGI_FORMAT_R24G8_TYPELESS;
	textureDescriptor.SampleDesc.Count = 1u;
	textureDescriptor.SampleDesc.Quality = 0u;
	textureDescriptor.Usage = D3D11_USAGE_DEFAULT;											//Default read and write access for GPU.	
	textureDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE;	//Texture will have depth data written to and depth data sampled from.
	textureDescriptor.CPUAccessFlags = 0u;													//No access for CPU.
	textureDescriptor.MiscFlags = 0u;														//No additional flags.

	Microsoft::WRL::ComPtr<ID3D11Texture2D> depthMapTexture;
	m_Hresult = device->CreateTexture2D(&textureDescriptor, nullptr, &depthMapTexture);
	if (FAILED(m_Hresult))
	{
		MessageBox(nullptr, L"Error creating depth map-texture.", L"ERROR", MB_OK);
		return false;
	}

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDescriptor;
	ZeroMemory(&depthStencilViewDescriptor, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	/*Format is of unsigned normalized value (so range 0 - 1), where depth component is accessed as a
	  24-bit floating point value and stencil component is accessed as an 8-bit unsigned integer value*/
	depthStencilViewDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDescriptor.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;		//Texture should be accessed as a Texture2D.
	depthStencilViewDescriptor.Texture2D.MipSlice = 0u;								//MIP irrelevant.

	m_Hresult = device->CreateDepthStencilView(depthMapTexture.Get(), &depthStencilViewDescriptor, &m_DepthStencilView);
	if (FAILED(m_Hresult))
	{
		MessageBox(nullptr, L"Error creating depth stencil view for shadow map", L"ERROR", MB_OK);
		return false;
	}

	D3D11_SHADER_RESOURCE_VIEW_DESC shaderResourceViewDescriptor;
	ZeroMemory(&shaderResourceViewDescriptor, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));

	/* 32-bit format, with 24 bits specifying an unsigned normalized integer in the red channel (->R<-24), and 8 unused typeless bits.*/
	shaderResourceViewDescriptor.Format = DXGI_FORMAT_R24_UNORM_X8_TYPELESS;
	shaderResourceViewDescriptor.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;		//Should be accessed as a Texture2D.
	shaderResourceViewDescriptor.Texture2D.MipLevels = 1u;							//Maximum number of mipmap levels.
	shaderResourceViewDescriptor.Texture2D.MostDetailedMip = 0u;					//Mipmap not of concern.

	m_Hresult = device->CreateShaderResourceView(depthMapTexture.Get(), &shaderResourceViewDescriptor, &m_DepthShaderResourceView);
	if (FAILED(m_Hresult))
	{
		MessageBox(nullptr, L"Error creating shader resource view for shadow map.", L"ERROR", MB_OK);
		return false;
	}

	m_ShadowMapSize = static_cast<float>(width);

	/*Shadow bias will be size of one texel*/
	m_ShadowBias = (1.0f / m_ShadowMapSize);

	return true;
}

const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShadowMap::GetShaderResourceView() const
{
	return m_DepthShaderResourceView;
}

ID3D11DepthStencilView* ShadowMap::GetDepthStencilView()
{
	return m_DepthStencilView.Get();
}

const D3D11_VIEWPORT& ShadowMap::GetViewPort() const
{
	return m_ViewPort;
}

const float& ShadowMap::GetShadowMapSize() const
{
	return m_ShadowMapSize;
}

const float& ShadowMap::GetShadowBias() const
{
	return m_ShadowBias;
}

const int& ShadowMap::GetPcfCount() const
{
	return m_PcfCount;
}
