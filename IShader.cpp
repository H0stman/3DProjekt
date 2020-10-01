#include "IShader.hpp"

IShader::~IShader() {
	shaderblob->Release();
}

ID3DBlob* IShader::GetShaderBlob() const 
{
	return shaderblob;
}