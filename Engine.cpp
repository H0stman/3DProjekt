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

	/*****RENDER TEXTURE CREATION*****/
	rendertexture = new Texture(width, height, device);

	/*****BLUR TARGET TEXTURE CREATION*****/
	blurtarget = new Texture(width, height, device);

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

	//Creating texturesampler
	D3D11_SAMPLER_DESC samplerDescriptor;
	ZeroMemory(&samplerDescriptor, sizeof(D3D11_SAMPLER_DESC));

	samplerDescriptor.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDescriptor.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDescriptor.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDescriptor.MinLOD = 0;
	samplerDescriptor.MaxLOD = D3D11_FLOAT32_MAX;

	hr = device->CreateSamplerState(&samplerDescriptor, &texturesampler);


	LoadDrawables();

	D3D11_BUFFER_DESC lightdesc = {};
	lightdesc.ByteWidth = sizeof(Light);
	lightdesc.MiscFlags = 0;
	lightdesc.StructureByteStride = 0;
	lightdesc.Usage = D3D11_USAGE_DYNAMIC;
	lightdesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	lightdesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	hr = device->CreateBuffer(&lightdesc, nullptr, &lightbuffer);
	assert(SUCCEEDED(hr));

	D3D11_BUFFER_DESC matrixBufferDesc = {};
	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(TransformationMatrices);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;
	hr = device->CreateBuffer(&matrixBufferDesc, nullptr, &matrixbuffer);
	assert(SUCCEEDED(hr));
	
	/** Setting up dynamic quad for rendering 2D images/textures **/
	float top = static_cast<float>(height) / 2.0f;
	float right = static_cast<float>(width) / 2.0f;
	float bottom = -top;
	float left = -right;
	float quad[] = {
		left, top, 0.0,				// Vertex
		0.0, 0.0,					// Texture coordinate
		0.0, 0.0, 1.0,				// Normal (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Tangent (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Bitangent (not used but necessary to comply with current input layout)
		right, top, 0.0,
		1.0, 0.0,
		0.0, 0.0, 1.0,				// Normal (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Tangent (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Bitangent (not used but necessary to comply with current input layout)
		left, bottom, 0.0,
		0.0, 1.0,
		0.0, 0.0, 1.0,				// Normal (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Tangent (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Bitangent (not used but necessary to comply with current input layout)
		right, bottom, 0.0,
		1.0, 1.0,
		0.0, 0.0, 1.0,				// Normal (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Tangent (not used but necessary to comply with current input layout)
		0.0, 0.0, 1.0,				// Bitangent (not used but necessary to comply with current input layout)
	};
	UINT vertexStride = 14 * sizeof(float);
	UINT vertexOffset = 0;
	UINT vertexCount = 4;

	D3D11_BUFFER_DESC quadDesc;
	ZeroMemory(&quadDesc, sizeof(D3D11_BUFFER_DESC));
	quadDesc.Usage = D3D11_USAGE_DYNAMIC;
	quadDesc.ByteWidth = vertexStride * vertexCount;
	quadDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	quadDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	quadDesc.MiscFlags = 0;
	quadDesc.StructureByteStride = 0;

	D3D11_SUBRESOURCE_DATA quadData;
	quadData.pSysMem = quad;
	quadData.SysMemPitch = 0;
	quadData.SysMemSlicePitch = 0;

	HRESULT HR = device->CreateBuffer(&quadDesc, &quadData, &render2Dquad);
	if (FAILED(HR))
		OutputDebugString(L"Error creating Quad-buffer");

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
	pixelshader2D->Release();
	vertexshader->Release();
	vertexshader2D->Release();
	csblurshader->Release();
	blobpixelvanilla->Release();
	blobpixel2D->Release();
	blobvertexvanilla->Release();
	blobvertex2D->Release();
	blobcsblur->Release();
	inputlayout->Release();
	lightbuffer->Release();
	matrixbuffer->Release();
	render2Dquad->Release();
	texturesampler->Release();
	if(rendertexture != nullptr) delete rendertexture;
	if(blurtarget != nullptr) delete blurtarget;
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

		OutputDebugString(L"Error compiling PixelShader Vanilla");
		errorBlob->Release();
	}

	/*****Pixelshader creation*****/
	hr = device->CreatePixelShader(blobpixelvanilla->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
		blobpixelvanilla->GetBufferSize(),													//Size of the compiled Pixel shader buffer.
		nullptr,																						//Advanced topic, not used here.
		&pixelshader);																				//Address of pointer to the Pixel VertexShader.
	if (FAILED(hr))
	{
		OutputDebugString(L"Error creating PixelShader Vanilla");
		errorBlob->Release();
	}


	/*****Pixelshader for 2D compilation*****/
	hr = D3DCompileFromFile(L"pixelshader2D.hlsl",		//Name of the pixel shader.
		nullptr,																			//PixelShader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,										//Will essentially find the file.
		"ps_2D_main",																		//Entry point for shader function.
		"ps_5_0",																		//Pixel shader target (version).
		flags,																			//Flags, in our case adding more debug output.
		0u,																				//Additional flags.
		&blobpixel2D,															//The pixel shader blob to be filled.
		&errorBlob);																	//Error blob that will catch additional error messages.
	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());			//Will yield additional debug information from Pixel shader.

		OutputDebugString(L"Error compiling PixelShader 2D");
		errorBlob->Release();
	}

	/*****Pixelshader creation*****/
	hr = device->CreatePixelShader(blobpixel2D->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
		blobpixel2D->GetBufferSize(),													//Size of the compiled Pixel shader buffer.
		nullptr,																						//Advanced topic, not used here.
		&pixelshader2D);																				//Address of pointer to the Pixel VertexShader.
	if (FAILED(hr))
	{
		OutputDebugString(L"Error creating PixelShader 2D");
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

		OutputDebugString(L"Error compiling VertexShader Vanilla");
		errorBlob->Release();
	}

	/*****Vertexshader creation*****/
	hr = device->CreateVertexShader(blobvertexvanilla->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
		blobvertexvanilla->GetBufferSize(),					//Size of the compiled Pixel shader buffer.
		nullptr,														//Advanced topic, not used here.
		&vertexshader);											//Address of pointer to the Pixel VertexShader.

	if (FAILED(hr))
	{
		OutputDebugString(L"Error creating VertexShader Vanilla");
		errorBlob->Release();
	}

	if(errorBlob)
		errorBlob->Release();

	/*****Vertexshader 2D compilation*****/
	hr = D3DCompileFromFile(L"vertexshader2D.hlsl",					//Name of the vertex shader.
		nullptr,																			//Vertexshader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,										//Will essentially find the file.
		"vs_2D_main",																		//Entry point for shader function.
		"vs_5_0",																		//Pixel shader target (version).
		flags,																			//Flags, in our case adding more debug output.
		0u,																				//Additional flags.
		&blobvertex2D,															//The pixel shader blob to be filled.
		&errorBlob);																	//Error blob that will catch additional error messages.

	if (FAILED(hr))
	{
		if (errorBlob != nullptr)
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());			//Will yield additional debug information from Pixel shader.

		OutputDebugString(L"Error compiling VertexShader 2D");
		errorBlob->Release();
	}

	/*****Vertexshader 2D creation*****/
	hr = device->CreateVertexShader(blobvertexvanilla->GetBufferPointer(),			//Pointer to the compiled Pixel shader buffer.
		blobvertexvanilla->GetBufferSize(),					//Size of the compiled Pixel shader buffer.
		nullptr,														//Advanced topic, not used here.
		&vertexshader2D);											//Address of pointer to the Pixel VertexShader.

	if (FAILED(hr))
	{
		OutputDebugString(L"Error creating VertexShader 2D");
		errorBlob->Release();
	}

	if(errorBlob)
		errorBlob->Release();

	/*****Computeshader compilation*****/
	HRESULT HR = D3DCompileFromFile(L"gaussianblurshader.hlsl",		//Name of the pixel shader.
		nullptr,										//ComputeShader macro, ignore.
		D3D_COMPILE_STANDARD_FILE_INCLUDE,				//Will essentially find the file.
		"cs_main",									//Entry point for shader function.
		"cs_5_0",										//Compute shader target (version).
		flags,											//Flags, in our case adding more debug output.
		0u,												//Additional flags.
		&blobcsblur,									//The compute shader blob to be filled.
		&errorBlob);									//Error blob that will catch additional error messages.
	if (FAILED(HR))
	{
		if (errorBlob != nullptr)
		{
			OutputDebugStringA((char*)errorBlob->GetBufferPointer());			//Will yield additional debug information from Compute shader.
		}
		OutputDebugStringA("Error compiling ComputeShader");
	}

	/*****Compute shader creation*****/
	HR = device->CreateComputeShader(blobcsblur->GetBufferPointer(),			//Pointer to the compiled Compute shader buffer.
		blobcsblur->GetBufferSize(),			//Size of the compiled Compute shader buffer.
		nullptr,									//Advanced topic, not used here.
		&csblurshader);						//Address of pointer to the Compute VertexShader.
	if (FAILED(HR))
	{
		OutputDebugString(L"Error creating ComputeShader");
	}
}

VOID Engine::VanillaRender()
{
	//Bind resources

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	context->VSSetShader(vertexshader, nullptr, 0u);
	context->PSSetShader(pixelshader, nullptr, 0u);
	context->PSSetSamplers(0u, 1u, &texturesampler);
	context->VSSetConstantBuffers(0u, 1u, &matrixbuffer);
	context->PSSetConstantBuffers(0u, 1u, &lightbuffer);
	context->PSSetConstantBuffers(1u, 1u, &matrixbuffer);
	context->RSSetViewports(1u, &defaultviewport);

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

VOID Engine::Render2D()
{
	// Set Shaders for 2D rendering
	context->VSSetShader(vertexshader2D, nullptr, 0u);
	context->PSSetShader(pixelshader2D, nullptr, 0u);
	
	//Update tranformation matrices.
	context->Map(matrixbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &transformresource);
	transform = (TransformationMatrices*)transformresource.pData;
	transform->worldmatrix = XMMatrixIdentity();
	transform->viewmatrix = XMMatrixIdentity(); // XMMatrixTranspose(camera.GetViewMatrix());
	transform->projectionmatrix = XMMatrixTranspose(camera.GetOrthoMatrix());
	context->Unmap(matrixbuffer, 0);

	context->VSSetConstantBuffers(0u, 1u, &matrixbuffer);
	context->PSSetConstantBuffers(0u, 1u, &lightbuffer);
	context->RSSetViewports(1u, &defaultviewport);

	/*****Set Stencil State to no depth*****/
	context->OMSetDepthStencilState(nozstencilstate, 1u);

	/*****Set Vertex Buffers*****/
	context->IASetVertexBuffers(0u,			//The startslot of the vertex buffer being used. 
		1u,											//Total number of vertex buffers.
		&render2Dquad,								//Address of pointer to Vertex buffer.
		&stride,										//The byte-stride of entities in the Vertex buffer, here vertices.
		&offset);									//Point of start of the first vertex to be used. Here all vertices are bound to buffer.
	/*****Set Primitive topology*****/
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);
	ID3D11ShaderResourceView* srv[] = { rendertexture->GetShaderResourceView() };
	context->PSSetShaderResources(0u, 1u, srv);
	context->Draw(4u, 0u);
}

VOID Engine::Blur(Texture* source, Texture* target)
{
	context->CSSetShader(csblurshader, nullptr, 0u);
	ID3D11RenderTargetView* unset[] = { nullptr };
	context->OMSetRenderTargets(0u, unset, nullptr);
	// Bind resources to pipeline
	ID3D11ShaderResourceView* srv[] = { source->GetShaderResourceView() };
	context->CSSetShaderResources(1u, 1u, srv);
	ID3D11UnorderedAccessView* uav[] = { target->GetUnorderedAccessView() };
	context->CSSetUnorderedAccessViews(0u, 1u, uav, nullptr);

	// Run the blur shader
	context->Dispatch(32u, 32u, 1u);

	// Unbind resources to pipeline
	srv[0] = nullptr;
	context->CSSetShaderResources(1u, 1u, srv);
	uav[0] = nullptr;
	context->CSSetUnorderedAccessViews(0u, 1u, uav, nullptr);
}

VOID Engine::SetRenderTargets(UINT target)
{
	if (target == 0) {
		context->OMSetRenderTargets(1u, &backbuffer, depthstencilview);
		context->OMSetDepthStencilState(defaultstencilstate, 0u);
	}
	if (target == 1) {
		ID3D11RenderTargetView* tgt[] = { rendertexture->GetRenderTargetView() };
		context->OMSetRenderTargets(1u, tgt, depthstencilview);
		context->OMSetDepthStencilState(defaultstencilstate, 0u);
	}
	if (target == 2) {
		context->OMSetRenderTargets(1u, &backbuffer, depthstencilview);
		context->OMSetDepthStencilState(nozstencilstate, 0u);
	}
}

VOID Engine::Update() 
{
	context->ClearDepthStencilView(depthstencilview, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0u);
	context->ClearRenderTargetView(backbuffer, clearcolour);

	context->ClearRenderTargetView(rendertexture->GetRenderTargetView(), clearcolour);
	
	camera.Update();
	SetRenderTargets(0u);	// 0 = backbuffer, 1 = render to rendertexture, 2 = backbuffer and no depth buffer
	VanillaRender();

	for (auto model : models) //TODO: Fix the incompatible input output structs in vertex and pixelshader for vanilla rendering.
	{
		if (model->IsClockwise())
			context->RSSetState(clocklwise);
		else
			context->RSSetState(counterclockwise);


		//Update tranformation matrices.
		context->Map(matrixbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &transformresource);
		transform = (TransformationMatrices*)transformresource.pData;
		transform->worldmatrix = model->GetWorldMatrix();
		transform->viewmatrix = XMMatrixTranspose(camera.GetViewMatrix());
		transform->projectionmatrix = XMMatrixTranspose(camera.GetProjectionMatrix());
		context->Unmap(matrixbuffer, 0);


		//Update light.
		context->Map(lightbuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &lightresouce);
		light = (Light*)lightresouce.pData;
		light->diffuseColour = XMFLOAT4(0.1f, 1.0f, 1.0f, 1.0f);
		light->pos = XMFLOAT3(10.0f, 10.0f, 0.0f);
		light->padding = 0.0f;
		context->Unmap(lightbuffer, 0);

		//Set texture.
		ID3D11ShaderResourceView* texture = model->GetTextures()->GetShaderResourceView();
		context->PSSetShaderResources(0u, 1u, &texture);

		context->IASetIndexBuffer(model->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0u);
		context->IASetVertexBuffers(0u, 1u, model->GetVertexBuffer(), &stride, &offset);
		context->DrawIndexed(model->GetIndexCount(), 0u, 0u);
	}

	//Blur(rendertexture, blurtarget);
	//SetRenderTargets(2u);
	//Render2D();

	swapchain->Present(1u, 0u);
}

VOID Engine::LoadDrawables() 
{
	models.push_back(new Terrain("heightmap.bmp", device));
	models.push_back(new Model("cube.obj", device));
}
