#include "IShader.hpp"

ID3DBlob* IShader::GetShaderBlob() const 
{
	return shaderblob.Get();
}