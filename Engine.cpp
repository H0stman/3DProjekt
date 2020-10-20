#include "Engine.hpp"

Engine::Engine(HWND hndl) : windowhandle(hndl), clearcolour{ 0.0f, 0.0f, 0.0f, 1.0f }
{
	RECT rect;
	GetClientRect(hndl, &rect);
	UINT width = rect.right - rect.left;
	UINT height = rect.bottom - rect.top;

	stride = sizeof(vertex);
	offset = 0u;

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
	swap_chain_descr.Windowed = TRUE;

	D3D_FEATURE_LEVEL feature_level[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_descr, &swapchain, &device, feature_level, &context);


	mouse.SetWindow(hndl);
	mouse.Get().SetMode(Mouse::MODE_ABSOLUTE);
	

	/***RENDER TARGET VIEW CREATION***/
	
	ID3D11Texture2D* pBackBuffer = nullptr, * pDepthStencilBuffer = nullptr;

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

	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
	depthStencilDesc.StencilEnable = TRUE;
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
	defaultviewport.Width = static_cast<FLOAT>(width);						//Width is the same as client window width.
	defaultviewport.Height = static_cast<FLOAT>(height);					//Height is the same as client window height.
	defaultviewport.MinDepth = 0.f;												//Min depth is 0.0 in D3D11.
	defaultviewport.MaxDepth = 1.0f;												//Max depth is 1.0 in D3D11.

	pBackBuffer->Release();
	pDepthStencilBuffer->Release();

	CreateRasterizerStates();
	CompileShaders();

	/*****Vertex Input Layout*****/
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA,0 },
		{"BITANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	/*****Input layout creation*****/
	hr = device->CreateInputLayout(iedesc,			//Pointer to the first element in the descriptor array.
		ARRAYSIZE(iedesc),								//Number of elements in descriptor array.
		blobvertexvanilla->GetBufferPointer(),		//Pointer to the compiled pixel shader buffer.
		blobvertexvanilla->GetBufferSize(),			//Size of the compiled pixel shader buffer.
		&inputlayout);										//Adress of pointer of yhe input layout. 

	context->IASetInputLayout(inputlayout);

	camera = Camera();

	context->VSSetShader(vertexshader, nullptr, 0u);
	context->PSSetShader(pixelshader, nullptr, 0u);

	LoadDrawables();

	D3D11_BUFFER_DESC desc = {};
	desc.ByteWidth = sizeof(Light);
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&desc, nullptr, &lightbuffer);
	assert(SUCCEEDED(hr));

	D3D11_MAPPED_SUBRESOURCE lghtresrc;

	
	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(TransformationMatrices);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	hr = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixbuffer);
	assert(SUCCEEDED(hr));
	
	

}

Engine::~Engine()
{
	for (auto obj : models)
		delete obj;
	device->Release();
	context->Release();
	swapchain->Release();
	backbuffer->Release();
	depthstencilview->Release();
	defaultstencilstate->Release();
	nozstencilstate->Release();
	clocklwise->Release();
	counterclockwise->Release();
	pixelshader->Release();
	vertexshader->Release();
	blobpixelvanilla->Release();
	blobvertexvanilla->Release();
	inputlayout->Release();
	lightbuffer->Release();
	matrixbuffer->Release();
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

VOID Engine::CreateRasterizerStates()
{
	//Create rasterizer states
	D3D11_RASTERIZER_DESC rdesc;

	ZeroMemory(&rdesc, sizeof(D3D11_RASTERIZER_DESC));
	rdesc.FillMode = D3D11_FILL_SOLID;
	rdesc.CullMode = D3D11_CULL_BACK;
	rdesc.FrontCounterClockwise = TRUE;
	device->CreateRasterizerState(&rdesc, &counterclockwise);

	rdesc.FrontCounterClockwise = FALSE;
	device->CreateRasterizerState(&rdesc, &clocklwise);
}

VOID Engine::CompileShaders()
{
	UINT flags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )
	flags |= D3DCOMPILE_DEBUG;											//Add more debug output.
#endif
	ID3DBlob* errorBlob;

	/*****Pixelshader compilation*****/
	HRESULT hr = D3DCompileFromFile(L"pixelshader_vanilla.hlsl",		//Name of the pixel shader.
		nullptr,																			//PixelShader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,										//Will essentially find the file.
		"ps_main",																		//Entry point for shader function.
		"ps_5_0",																		//Pixel shader target (version).
		flags,																			//Flags, in our case adding more debug output.
		0u,																				//Additional flags.
		&blobpixelvanilla,															//The pixel shader blob to be filled.
		&errorBlob);																	//Error blob that will catch additional error messages.
	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());			//Will yield additional debug information from Pixel shader.

		MessageBox(nullptr, L"Error compiling Pixel shader.", L"ERROR", MB_OK);
		errorBlob->Release();
	}

	/*****Pixelshader creation*****/
	hr = device->CreatePixelShader(blobpixelvanilla->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
		blobpixelvanilla->GetBufferSize(),				//Size of the compiled Pixel shader buffer.
		nullptr,													//Advanced topic, not used here.
		&pixelshader);											//Address of pointer to the Pixel VertexShader.
	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Error creating Pixel VertexShader.", L"ERROR", MB_OK);
		errorBlob->Release();
	}


	/*****Vertexshader compilation*****/
	hr = D3DCompileFromFile(L"vertexshader_vanilla.hlsl",					//Name of the vertex shader.
		nullptr,																			//Vertexshader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,										//Will essentially find the file.
		"vs_main",																		//Entry point for shader function.
		"vs_5_0",																		//Pixel shader target (version).
		flags,																			//Flags, in our case adding more debug output.
		0u,																				//Additional flags.
		&blobvertexvanilla,															//The pixel shader blob to be filled.
		&errorBlob);																	//Error blob that will catch additional error messages.

	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());			//Will yield additional debug information from Pixel shader.

		MessageBox(nullptr, L"Error compiling Pixel shader.", L"ERROR", MB_OK);
		errorBlob->Release();
	}

	/*****Vertexshader creation*****/
	hr = device->CreateVertexShader(blobvertexvanilla->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
		blobvertexvanilla->GetBufferSize(),					//Size of the compiled Pixel shader buffer.
		nullptr,														//Advanced topic, not used here.
		&vertexshader);											//Address of pointer to the Pixel VertexShader.

	if (FAILED(hr))
	{
		MessageBox(nullptr, L"Error creating Pixel VertexShader.", L"ERROR", MB_OK);
		errorBlob->Release();
	}

	if(errorBlob)
		errorBlob->Release();

}

VOID Engine::VanillaRender()
{
	//Bind resources

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->VSSetShader(vertexshader, nullptr, 0u);
	context->PSSetShader(pixelshader, nullptr, 0u);
	context->OMSetRenderTargets(1u, &backbuffer, depthstencilview);
	context->VSSetConstantBuffers(0u, 1u, &matrixbuffer);
	context->PSSetConstantBuffers(0u, 1u, &lightbuffer);
	context->RSSetViewports(1u, &defaultviewport);
	context->OMSetDepthStencilState(defaultstencilstate, 0u);

	/*if (ppRenderTargets == nullptr)
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
	context->ClearDepthStencilView(depthstencilview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	context->ClearRenderTargetView(backbuffer, clearcolour);

	
	
	camera.Update();
	VanillaRender();

	for (auto model : models) //TODO: Fix the incompatible input output structs in vertex and pixelshader for vanilla rendering.
	{
		if (model->IsClockwise())
			context->RSSetState(clocklwise);
		else
			context->RSSetState(counterclockwise);

		context->Map(matrixbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
		transform = (TransformationMatrices*)mappedResource.pData;
		transform->worldmatrix = model->GetWorldMatrix();
		transform->viewmatrix = XMMatrixTranspose(camera.GetViewMatrix());
		transform->projectionmatrix = XMMatrixTranspose(camera.GetProjectionMatrix());
		context->Unmap(matrixbuffer, 0);
		context->IASetIndexBuffer(model->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
		context->IASetVertexBuffers(0u, 1u, model->GetVertexBuffer(), &stride, &offset);
		context->DrawIndexed(model->GetIndexCount(), 0u, 0u);
	}



	swapchain->Present(1u, 0u);
}

VOID Engine::LoadDrawables() 
{
	models.push_back(new Terrain("heightmap.bmp", device));
	models.push_back(new Model("cube.obj", device));
}
