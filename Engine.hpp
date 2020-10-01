#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <wrl\client.h>
#include <string>
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>
#include "Window.hpp"

#include "Camera.hpp"
#include <DirectXMath.h>

using namespace DirectX;

class Engine
{
	static Engine* instance;
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	IDXGISwapChain* swapchain;

	enum RenderType { VANILLA, DEFERRED, SHADOWMAP, BLUR };

	Mouse mouse;
	Keyboard keyboard;

	Engine();
	~Engine();
	

public:
	
	Camera camera;
	Window window;
	static Engine *GetInstance();
	void Release();
	void UpdateCameraPosition();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
};
