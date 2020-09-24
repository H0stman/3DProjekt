#pragma once
#include "IShader.hpp"

class DomainShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11DomainShader> m_DomainShader;
private:
	DomainShader(const DomainShader& origShader) = delete;
	DomainShader& operator=(const DomainShader& origShader) = delete;
public:
	DomainShader() = default;
	virtual ~DomainShader() = default;
	virtual const bool Initialize(ID3D11Device* device, Shader_Setup_Details &setup);

	virtual void SetShader(ID3D11DeviceContext* deviceContext) const;
	virtual void UnSetShader(ID3D11DeviceContext* deviceContext) const override;
};
