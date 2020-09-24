#pragma once
#include "IShader.hpp"

class VertexShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11VertexShader> m_VertexShader;
private:
	VertexShader(const VertexShader& origShader) = delete;
	VertexShader& operator=(const VertexShader& origShader) = delete;
public:
	VertexShader() = default;
	virtual ~VertexShader() = default;
	virtual const bool Initialize(ID3D11Device* device, Shader_Setup_Details &setup) override;

	virtual void SetShader(ID3D11DeviceContext* deviceContext) const override;
	virtual void UnSetShader(ID3D11DeviceContext* deviceContext) const override;
};