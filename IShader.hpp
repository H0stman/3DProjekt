// #define NOMINMAX
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
	Microsoft::WRL::ComPtr<ID3DBlob> m_ShaderBlob;
private:
	IShader(const IShader& origShader) = delete;
	IShader& operator=(const IShader& origShader) = delete;
public:
	IShader() = default;
	virtual ~IShader() = default;
	virtual const bool Initialize(ID3D11Device* device, Shader_Setup_Details &setup) = 0;

	virtual void SetShader(ID3D11DeviceContext* deviceContext) const = 0;
	virtual void UnSetShader(ID3D11DeviceContext* deviceContext) const = 0;
	ID3DBlob* GetShaderBlob() const;
};