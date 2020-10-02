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

void VanillaRenderer::Draw(UINT numDrawables, IDrawable** ppDrawables, UINT numTargets, Texture **ppRenderTargets) {
	ID3D11DeviceContext* context = Engine::GetInstance()->GetContext();
	pixelshader.SetShader();
	if (ppRenderTargets == nullptr) {
		context->OMSetRenderTargets(1, /*backbuffer*/, /*depthstencil*/);
	}
	else {
		std::vector<ID3D11RenderTargetView*> targets;
		for (UINT i = 0; i < numTargets; ++i)
			targets.push_back(ppRenderTargets[i]->GetRenderTargetView());
		context->OMSetRenderTargets(numTargets, &targets[0], /*depthstencil*/);
	}
	for (UINT object = 0; object < numDrawables; ++object) {
		UINT i = 0;
		context->DrawIndexed(ppDrawables[i]->GetIndexCount(), ppDrawables[i]->GetILocation(), ppDrawables[i]->GetVLocation());
	}
}
