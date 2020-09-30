#pragma once
#include "IShader.hpp"

class HullShader : public IShader
{
private:
	Microsoft::WRL::ComPtr<ID3D11HullShader> hullshader;
private:
	HullShader(const HullShader& origShader) = delete;
	HullShader& operator=(const HullShader& origShader) = delete;
public:
	HullShader() = default;
	virtual ~HullShader() = default;
	virtual const bool Initialize(ID3D11Device* device, Shader_Setup_Details &setup);

	virtual void SetShader(ID3D11DeviceContext* deviceContext) const;
	virtual void UnSetShader(ID3D11DeviceContext* deviceContext) const override;
};
