#pragma once
#include "IShader.hpp"

class VertexShader : public IShader
{
private:
	ID3D11VertexShader *vertexshader;
private:
	VertexShader(const VertexShader& origShader) = delete;
	VertexShader& operator=(const VertexShader& origShader) = delete;
public:
	VertexShader() = default;
	virtual ~VertexShader() = default;
	virtual const bool Initialize(Shader_Setup_Details &setup) override;

	virtual void SetShader() const override;
	virtual void UnSetShader() const override;
};