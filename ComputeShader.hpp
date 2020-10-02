#pragma once
#include "IShader.hpp"

class ComputeShader : public IShader
{
private:
	ID3D11ComputeShader *computeshader;
private:
	ComputeShader(const ComputeShader& origShader) = delete;
	ComputeShader& operator=(const ComputeShader& origShader) = delete;
public:
	ComputeShader() = default;
	virtual ~ComputeShader() = default;
	virtual const bool Initialize(Shader_Setup_Details &setup) override;

	virtual void SetShader() const override;
	virtual void UnSetShader() const override;
};
