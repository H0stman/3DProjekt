#pragma once
#include "Engine.hpp"
#include <d3dcompiler.h>

struct Shader_Setup_Details {
	LPCWSTR fileName;
	D3D_SHADER_MACRO *defines;
	ID3DInclude *include;
	LPCSTR entryPoint;
	LPCSTR target;
	UINT flags1;
	UINT flags2;
};

class IShader
{
protected:
	ID3DBlob *shaderblob;
private:
	IShader(const IShader& origShader) = delete;
	IShader& operator=(const IShader& origShader) = delete;
public:
	IShader() = default;
	~IShader();
	virtual const bool Initialize(Shader_Setup_Details &setup) = 0;

	virtual void SetShader() const = 0;
	virtual void UnSetShader() const = 0;
	ID3DBlob* GetShaderBlob() const;
};