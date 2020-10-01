#pragma once
#include "IShader.hpp"

class PixelShader : public IShader
{
private:
	ID3D11PixelShader *pixelshader;
private:
	PixelShader(const PixelShader& origShader) = delete;
	PixelShader& operator=(const PixelShader& origShader) = delete;
public:
	PixelShader() = default;
	~PixelShader();
	const bool Initialize(Shader_Setup_Details &setup) override;

	 void SetShader() const override;
	 void UnSetShader() const override;
};