#include "VertexShader.hpp"

const bool VertexShader::Initialize(ID3D11Device* device, Shader_Setup_Details &setup)
{
	ID3DBlob *errorBlob;

	/***Vertex shader compilation***/
	HRESULT HR = D3DCompileFromFile( setup.fileName,	//Name of the vertex shader file.
									 setup.defines,		//VertexShader macro, ignore.
									 setup.include,		//Will essentially find the shader file.
									 setup.entryPoint,	//Entry point for shader function.
									 setup.target,		//Vertex shader revision.
									 setup.flags1,		//Flags, in our case adding more debug output.
									 setup.flags2,		//Additional flags.
									 &shaderblob,		//Our vertex shader blob to be filled.
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
	HR = device->CreateVertexShader( shaderblob->GetBufferPointer(),	//Pointer to the compiled Vertex shader.
									   shaderblob->GetBufferSize(),		//Size of the compiled shader.
									   nullptr,									//Advanced topic, not used here.
									   &vertexshader);						//Address of the pointer to the vertex shader interface.
	if (FAILED(HR))
	{
		MessageBox(nullptr, L"Error creating vertex shader.", L"ERROR", MB_OK);
		errorBlob->Release();
		return false;
	}
	
	if(errorBlob) errorBlob->Release();
	return true;
}

void VertexShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
	/*****Setting the vertex shader*****/
	deviceContext->VSSetShader(vertexshader.Get(), nullptr, 0u);
}

void VertexShader::UnSetShader(ID3D11DeviceContext* deviceContext) const
{
	/*****Setting the vertex shader*****/
	deviceContext->VSSetShader(nullptr, nullptr, 0u);
}