#include "HullShader.hpp"

const bool HullShader::Initialize(Shader_Setup_Details &setup)
{
	ID3D11Device* device = Engine::GetInstance()->GetDevice();
	ID3DBlob *errorBlob;

	/***Vertex shader compilation***/
	HRESULT HR = D3DCompileFromFile( setup.fileName,	//Name of the vertex shader file.
									 setup.defines,		//HullShader macro, ignore.
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
	HR = device->CreateHullShader( shaderblob->GetBufferPointer(),	//Pointer to the compiled Vertex shader.
									   shaderblob->GetBufferSize(),		//Size of the compiled shader.
									   nullptr,									//Advanced topic, not used here.
									   &hullshader);						//Address of the pointer to the vertex shader interface.
	if (FAILED(HR))
	{
		OutputDebugString(L"Error creating vertex shader.\n");
		errorBlob->Release();
		return false;
	}
	
	if(errorBlob) errorBlob->Release();
	return true;
}

void HullShader::SetShader() const
{
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	/*****Setting the vertex shader*****/
	context->HSSetShader(hullshader, nullptr, 0u);
}

void HullShader::UnSetShader() const
{
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	/*****Setting the vertex shader*****/
	context->HSSetShader(nullptr, nullptr, 0u);
}
