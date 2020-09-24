#pragma once
#include "IShader.hpp"

class ComputeShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11ComputeShader> m_ComputeShader;
private:
	ComputeShader(const ComputeShader& origShader) = delete;
	ComputeShader& operator=(const ComputeShader& origShader) = delete;
public:
	ComputeShader() = default;
	virtual ~ComputeShader() = default;
	virtual const bool Initialize(ID3D11Device* device, Shader_Setup_Details &setup) override;

	virtual void SetShader(ID3D11DeviceContext* deviceContext) const override;
	virtual void UnSetShader(ID3D11DeviceContext* deviceContext) const override;
};
