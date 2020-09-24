#pragma once
#include "IShader.hpp"

class PixelShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_PixelShader;
private:
	PixelShader(const PixelShader& origShader) = delete;
	PixelShader& operator=(const PixelShader& origShader) = delete;
public:
	PixelShader() = default;
	virtual ~PixelShader() = default;
	virtual const bool Initialize(ID3D11Device* device, Shader_Setup_Details &setup) override;

	virtual void SetShader(ID3D11DeviceContext* deviceContext) const override;
	virtual void UnSetShader(ID3D11DeviceContext* deviceContext) const override;
};