#include "HullShader.hpp"

const bool HullShader::Initialize(ID3D11Device* device, Shader_Setup_Details &setup)
{
	ID3DBlob *errorBlob;

	/***Vertex shader compilation***/
	HRESULT HR = D3DCompileFromFile( setup.fileName,	//Name of the vertex shader file.
									 setup.defines,		//HullShader macro, ignore.
									 setup.include,		//Will essentially find the shader file.
									 setup.entryPoint,	//Entry point for shader function.
									 setup.target,		//Vertex shader revision.
									 setup.flags1,		//Flags, in our case adding more debug output.
									 setup.flags2,		//Additional flags.
									 &m_ShaderBlob,		//Our vertex shader blob to be filled.
									 &errorBlob);		//Error blob that will catch additional error messages.
	if (FAILED(HR))
	{
		if (errorBlob != nullptr)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());		//Will yield additional debug information from Vertex shader.
		}
		MessageBox(nullptr, L"Error compiling vertex shader.", L"ERROR", MB_OK);
		errorBlob->Release();
		return false;
	}
	/*****Vertexshader creation*****/
	HR = device->CreateHullShader( m_ShaderBlob->GetBufferPointer(),	//Pointer to the compiled Vertex shader.
									   m_ShaderBlob->GetBufferSize(),		//Size of the compiled shader.
									   nullptr,									//Advanced topic, not used here.
									   &m_HullShader);						//Address of the pointer to the vertex shader interface.
	if (FAILED(HR))
	{
		MessageBox(nullptr, L"Error creating vertex shader.", L"ERROR", MB_OK);
		errorBlob->Release();
		return false;
	}
	
	if(errorBlob) errorBlob->Release();
	return true;
}

void HullShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
	/*****Setting the vertex shader*****/
	deviceContext->HSSetShader(m_HullShader.Get(), nullptr, 0u);
}

void HullShader::UnSetShader(ID3D11DeviceContext* deviceContext) const
{
	/*****Setting the vertex shader*****/
	deviceContext->HSSetShader(nullptr, nullptr, 0u);
}
