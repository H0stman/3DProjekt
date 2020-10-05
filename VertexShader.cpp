#include "VertexShader.hpp"

const bool VertexShader::Initialize(Shader_Setup_Details &setup)
{
	ID3D11Device* device = Engine::GetInstance()->GetDevice();
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
		OutputDebugString(L"Error compiling vertex shader.\n");
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
		OutputDebugString(L"Error creating vertex shader.\n");
		errorBlob->Release();
		return false;
	}
	
	if(errorBlob) errorBlob->Release();
	return true;
}

void VertexShader::SetShader() const
{
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	/*****Setting the vertex shader*****/
	context->VSSetShader(vertexshader, nullptr, 0u);
}

void VertexShader::UnSetShader() const
{
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	/*****Setting the vertex shader*****/
	context->VSSetShader(nullptr, nullptr, 0u);
}