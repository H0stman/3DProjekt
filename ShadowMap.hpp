#pragma once
#include <wrl/client.h>
#include <d3d11.h>

class ShadowMap
{
private:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_DepthShaderResourceView;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_DepthStencilView;
	D3D11_VIEWPORT m_ViewPort;
	HRESULT m_Hresult;
	float m_ShadowMapSize;
	float m_ShadowBias;
	int m_PcfCount;
private:
	ShadowMap(const ShadowMap& origShadowMap) = delete;
	ShadowMap& operator=(const ShadowMap& origShadowMap) = delete;
public:
	ShadowMap();
	virtual ~ShadowMap() = default;
	bool Initialize(ID3D11Device* device, const unsigned int& width, const unsigned int& height);

	[[nodiscard]] const Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetShaderResourceView() const;
	[[nodiscard]] ID3D11DepthStencilView* GetDepthStencilView();
	[[nodiscard]] const D3D11_VIEWPORT& GetViewPort() const;
	[[nodiscard]] const float& GetShadowMapSize() const;
	[[nodiscard]] const float& GetShadowBias() const;
	[[nodiscard]] const int& GetPcfCount() const;
};