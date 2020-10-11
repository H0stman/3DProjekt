#pragma once
#include "IShader.hpp"

class DomainShader : public IShader
{
private:
	ID3D11DomainShader* domainshader;
private:
	DomainShader(const DomainShader& origShader) = delete;
	DomainShader& operator=(const DomainShader& origShader) = delete;
public:
	DomainShader() = default;
	virtual ~DomainShader();
	virtual const bool Initialize(Shader_Setup_Details &setup);

	virtual void SetShader() const;
	virtual void UnSetShader() const override;
};
