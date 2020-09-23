#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include "Window.hpp"

class Engine
{
	static Engine* instance;
	ID3D11Device *device;
	ID3D11DeviceContext *devicecontext;
	IDXGISwapChain* swapchain;

	enum RenderType { VANILLA, DEFERRED, SHADOWMAP, BLUR };


	Engine();
	~Engine();

public:
	Window wnd;
	static Engine *getInstance();

};
