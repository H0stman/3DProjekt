#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>
#include <string>
#include <vector>


#include "Window.hpp"
#include "Camera.hpp"
#include "IDrawable.hpp"

using namespace DirectX;

class Engine
{
	static Engine* instance;
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	IDXGISwapChain* swapchain;

	Mouse mouse;
	Keyboard keyboard;

	Engine();

public:
	
	enum RenderType { VANILLA = 1, DEFERRED = 2, SHADOWMAP = 4, BLUR = 8 };

	Camera camera;
	Window window;

	static Engine *GetInstance();
	void UpdateCameraPosition();
	~Engine();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
};
