#include "VanillaRenderer.hpp"

VanillaRenderer::VanillaRenderer()
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
	#if defined( DEBUG ) || defined( _DEBUG )
		flags |= D3DCOMPILE_DEBUG;				//Add more debug output.
	#endif	

	Shader_Setup_Details setup = {
		L"pixelshader_vanilla.hlsl",			//Name of the vertex shader file.
		nullptr,								//VertexShader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,		//Will essentially find the shader file.
		"ps_main",								//Entry point for shader function.
		"ps_5_0",								//Vertex shader of 5.0 revision.
		flags,									//Flags, in our case adding more debug output.
		0u };									//Additional flags, ignore.

	pixelshader.Initialize(setup);
}
