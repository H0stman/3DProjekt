#include "PixelShader.hpp"

const bool PixelShader::Initialize(ID3D11Device* device, Shader_Setup_Details &setup)
{
	ID3DBlob *errorBlob;

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
								      shaderblob->GetBufferSize(),			//Size of the compiled Pixel shader buffer.
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

void PixelShader::SetShader(ID3D11DeviceContext* deviceContext) const
{
	/*****Setting the Pixel shader*****/
	deviceContext->PSSetShader(pixelshader.Get(), nullptr, 0u);
}

void PixelShader::UnSetShader(ID3D11DeviceContext* deviceContext) const
{
	/*****Setting the Pixel shader*****/
	deviceContext->PSSetShader(nullptr, nullptr, 0u);
}
