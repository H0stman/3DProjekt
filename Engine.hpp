#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include "Window.hpp"
#include <wrl\client.h>
#include <DirectXMath.h>

using namespace DirectX;

class Engine
{
	static Engine* instance;
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	IDXGISwapChain* swapchain;

	enum RenderType { VANILLA, DEFERRED, SHADOWMAP, BLUR };


	Engine();
	~Engine();

public:
	Window wnd;
	static Engine *GetInstance();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
};
