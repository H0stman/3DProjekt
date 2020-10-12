#include "Engine.hpp"

Engine::Engine(HWND hndl) : windowhandle(hndl), clearcolour{ 0.0f, 1.0f,0.0f,1.0f }
{

	RECT rect;
	GetClientRect(hndl, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;
	DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
	swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_descr.SampleDesc.Count = 1;
	swap_chain_descr.SampleDesc.Quality = 0;
	swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_descr.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_descr.BufferCount = 2;
	swap_chain_descr.OutputWindow = hndl;
	swap_chain_descr.Windowed = true;

	D3D_FEATURE_LEVEL feature_level[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_descr, &swapchain, &device, feature_level, &context);


	mouse.SetWindow(hndl);
	mouse.Get().SetMode(Mouse::MODE_RELATIVE);
	

	/***RENDER TARGET VIEW CREATION***/
	
	ID3D11Texture2D* pBackBuffer, * pDepthStencilBuffer;

	hr = swapchain->GetBuffer(0u, __uuidof(ID3D11Texture2D), (void**)&pBackBuffer);
	if (FAILED(hr))
		OutputDebugString(L"Error generating back buffer for Render target view.\n");

	hr = device->CreateRenderTargetView(pBackBuffer, nullptr, &backbuffer);
	if (FAILED(hr))
		OutputDebugString(L"Error generating back buffer for Render target view.\n");

	/*****DEPTH/STENCIL VIEW CREATION*****/
	D3D11_TEXTURE2D_DESC depthBufferDescriptor;
	ZeroMemory(&depthBufferDescriptor, sizeof(D3D11_TEXTURE2D_DESC));

	depthBufferDescriptor.Width = width;									//Width of texture in texels.
	depthBufferDescriptor.Height = height;									//Height of texture in texels.
	depthBufferDescriptor.MipLevels = 1u;									//Our texture only needs one mip-level.
	depthBufferDescriptor.ArraySize = 1u;									//We only need one texture for depth/stencil view.
	depthBufferDescriptor.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;	//Format specifier for depth/stencil buffer.
	depthBufferDescriptor.SampleDesc.Count = 1u;							//Do not use MSAAXX(Change...?).
	depthBufferDescriptor.SampleDesc.Quality = 0u;						//Do not use MSAAXX(Change...?).
	depthBufferDescriptor.Usage = D3D11_USAGE_DEFAULT;					//GPU will be doing all the reading and writing.
	depthBufferDescriptor.BindFlags = D3D11_BIND_DEPTH_STENCIL;		//This is a depth/stencil buffer and should be bound as such.
	depthBufferDescriptor.CPUAccessFlags = 0u;							//The CPU will NOT be reading or writing to depth/stencil buffer.
	depthBufferDescriptor.MiscFlags = 0u;                          //Optional flags not set for depth/stencil buffer.

	hr = device->CreateTexture2D(&depthBufferDescriptor, nullptr, &pDepthStencilBuffer);
	if (FAILED(hr))
		OutputDebugString(L"Error generating buffer for depth/stencil view.\n");

	hr = device->CreateDepthStencilView(pDepthStencilBuffer, nullptr, &depthstencilview);
	if (FAILED(hr))
		OutputDebugString(L"Error generating depth/stencil view.\n");

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;
	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	device->CreateDepthStencilState(&depthStencilDesc, &defaultstencilstate);
	depthStencilDesc.DepthEnable = false;
	device->CreateDepthStencilState(&depthStencilDesc, &nozstencilstate);
	context->OMSetDepthStencilState(defaultstencilstate, 0u);

	//rendertexture.Initialize(device, window.GetWidth(), window.GetHeight());
	//blurtexture.Initialize(device, window.GetWidth(), window.GetHeight());

	/*****SETTING THE VIEWPORT*****/
	ZeroMemory(&defaultviewport, sizeof(D3D11_VIEWPORT));

	defaultviewport.TopLeftX = 0u;												//It starts at 0 relative to the client area rectangle.
	defaultviewport.TopLeftY = 0u;												//It starts at 0 relative to the client area rectangle.
	defaultviewport.Width = static_cast<float>(width);						//Width is the same as client window width.
	defaultviewport.Height = static_cast<float>(height);					//Height is the same as client window height.
	defaultviewport.MinDepth = 0.f;												//Min depth is 0.0 in D3D11.
	defaultviewport.MaxDepth = 1.0f;												//Max depth is 1.0 in D3D11.

	pBackBuffer->Release();
	pDepthStencilBuffer->Release();


	Shader_Setup_Details setup =
	{
		L"pixelshader_vanilla.hlsl",			//Name of the vertex shader file.
		nullptr,										//VertexShader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//Will essentially find the shader file.
		"ps_main",									//Entry point for shader function.
		"ps_5_0",									//Vertex shader of 5.0 revision.
		flags,										//Flags, in our case adding more debug output.
		0u };											//Additional flags, ignore.

	vanillapixelshader.Initialize(setup);

	Shader_Setup_Details setup =
	{
		L"vertexshader_vanilla.hlsl",			//Name of the vertex shader file.
		nullptr,										//VertexShader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,	//Will essentially find the shader file.
		"vs_main",									//Entry point for shader function.
		"vs_5_0",									//Vertex shader of 5.0 revision.
		flags,										//Flags, in our case adding more debug output.
		0u };											//Additional flags, ignore.

	vanillavertexshader.Initialize(setup);

}

Engine::~Engine()
{
	device->Release();
	context->Release();
	swapchain->Release();
	backbuffer->Release();
	depthstencilview->Release();
	defaultstencilstate->Release();
	nozstencilstate->Release();
}

BOOL Engine::Run()
{
	auto kbstate = keyboard.GetState();
	if (kbstate.Escape)
	{
		DestroyWindow(windowhandle);
		PostQuitMessage(0);
		return FALSE;
	}
	else
		return TRUE;
}

ID3D11Device* Engine::GetDevice()
{
	return device;
}

ID3D11DeviceContext* Engine::GetContext()
{
	return context;
}

ID3D11RenderTargetView* Engine::GetBackbuffer()
{
	return backbuffer;
}

ID3D11DepthStencilView* Engine::GetDepthStencil()
{
	return depthstencilview;
}

VOID Engine::VanillaRender()
{
	/*vanillapixelshader.SetShader();
	if (ppRenderTargets == nullptr)
	{
		ID3D11RenderTargetView* backbufferarr[] = { backbuffer };
		context->OMSetRenderTargets(1, backbufferarr, depthstencilview);
	}*/
	/*else
	{
		std::vector<ID3D11RenderTargetView*> targets;
		for (UINT i = 0; i < numTargets; ++i)
			targets.push_back(ppRenderTargets[i]->GetRenderTargetView());
		engine->GetContext()->OMSetRenderTargets(numTargets, &targets[0], engine->GetDepthStencil());
	}*/
	/*for (UINT object = 0; object < numDrawables; ++object)
	{
		UINT i = 0;
		engine->GetContext()->DrawIndexed(ppDrawables[i]->GetIndexCount(), ppDrawables[i]->GetStartIndexLocation(), ppDrawables[i]->GetBaseVertexLocation());
	}*/
}

VOID Engine::Update() 
{
	context->ClearRenderTargetView(backbuffer, clearcolour);
	swapchain->Present(1u, 0u);
}
