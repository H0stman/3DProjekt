#include "ComputeShader.hpp"

const bool ComputeShader::Initialize(Shader_Setup_Details &setup)
{
	ID3D11Device* device = Engine::GetInstance()->GetDevice();
	ID3DBlob *pErrorBlob;

	/*****Computeshader compilation*****/
	HRESULT HR = D3DCompileFromFile( setup.fileName,		//Name of the pixel shader.
									 setup.defines,			//ComputeShader macro, ignore.
									 setup.include,			//Will essentially find the file.
									 setup.entryPoint,		//Entry point for shader function.
									 setup.target,			//Compute shader target (version).
									 setup.flags1,			//Flags, in our case adding more debug output.
									 setup.flags2,			//Additional flags.
									 &shaderblob,			//The compute shader blob to be filled.
									 &pErrorBlob);			//Error blob that will catch additional error messages.
	if (FAILED(HR))
	{
		if (pErrorBlob != nullptr)
		{
			OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());			//Will yield additional debug information from Compute shader.
		}
		MessageBox(nullptr, L"Error compiling Compute shader.", L"ERROR", MB_OK);
		return false;
	}

	/*****Compute shader creation*****/
	HR = device->CreateComputeShader( shaderblob->GetBufferPointer(),			//Pointer to the compiled Compute shader buffer.
								      shaderblob->GetBufferSize(),			//Size of the compiled Compute shader buffer.
								      nullptr,									//Advanced topic, not used here.
								      &computeshader);						//Address of pointer to the Compute VertexShader.
	if (FAILED(HR))
	{
		MessageBox(nullptr, L"Error creating ComputeShader", L"ERROR", MB_OK);
		return false;
	}
	return true;
}

void ComputeShader::SetShader() const
{
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	/*****Setting the Compute shader*****/
	context->CSSetShader(computeshader.Get(), nullptr, 0u);
}

void ComputeShader::UnSetShader() const
{
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	/*****Setting the Compute shader*****/
	context->CSSetShader(nullptr, nullptr, 0u);
}
