#include "PixelShader.hpp"

PixelShader::~PixelShader()
{
	pixelshader->Release();
}

const bool PixelShader::Initialize(Shader_Setup_Details &setup)
{
	ID3DBlob *errorBlob;
	ID3D11Device *device = Engine::GetInstance()->GetDevice();

	/*****Pixelshader compilation*****/
	HRESULT HR = D3DCompileFromFile( setup.fileName,		//Name of the pixel shader.
									 setup.defines,			//PixelShader macro, ignore.
									 setup.include,			//Will essentially find the file.
									 setup.entryPoint,		//Entry point for shader function.
									 setup.target,			//Pixel shader target (version).
									 setup.flags1,			//Flags, in our case adding more debug output.
									 setup.flags2,			//Additional flags.
									 &shaderblob,			//The pixel shader blob to be filled.
									 &errorBlob);			//Error blob that will catch additional error messages.
	if (FAILED(HR))
	{
		if (errorBlob != nullptr)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());			//Will yield additional debug information from Pixel shader.
		}
		MessageBox(nullptr, L"Error compiling Pixel shader.", L"ERROR", MB_OK);
		errorBlob->Release();
		return false;
	}

	/*****Pixel shader creation*****/
	HR = device->CreatePixelShader(   shaderblob->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
								      shaderblob->GetBufferSize(),				//Size of the compiled Pixel shader buffer.
								      nullptr,									//Advanced topic, not used here.
								      &pixelshader);							//Address of pointer to the Pixel VertexShader.
	if (FAILED(HR))
	{
		MessageBox(nullptr, L"Error creating Pixel VertexShader", L"ERROR", MB_OK);
		errorBlob->Release();
		return false;
	}

	if(errorBlob) errorBlob->Release();
	return true;
}

void PixelShader::SetShader() const
{
	/*****Setting the Pixel shader*****/
	Engine::GetInstance()->GetContext()->PSSetShader(pixelshader, nullptr, 0u);
}

void PixelShader::UnSetShader() const
{
	/*****Setting the Pixel shader*****/
	Engine::GetInstance()->GetContext()->PSSetShader(nullptr, nullptr, 0u);
}
