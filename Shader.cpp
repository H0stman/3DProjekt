#include "Shader.h"

ID3DBlob* Shader::GetShaderBlob() const 
{
	return m_ShaderBlob.Get();
}