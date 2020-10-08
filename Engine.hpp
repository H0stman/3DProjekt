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
#include "Texture.hpp"
#include "VanillaRenderer.hpp"

using namespace DirectX;

class Engine
{
	static Engine* instance;
	ID3D11Device *device;
	ID3D11DeviceContext *context;
	IDXGISwapChain* swapchain;
	D3D11_VIEWPORT defaultviewport;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthstencilview;
	ID3D11DepthStencilState* defaultstencilstate;
	ID3D11DepthStencilState* nozstencilstate;

	Mouse mouse;
	Keyboard keyboard;

	VanillaRenderer vanilla;

	Engine();

public:
	
	enum RenderType { VANILLA = 1, DEFERRED = 2, SHADOWMAP = 4, BLUR = 8 };

	Camera camera;
	Window window;

	static Engine *GetInstance();
	BOOL Run();
	VOID UpdateCameraPosition();
	~Engine();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	ID3D11RenderTargetView* GetBackbuffer();
	ID3D11DepthStencilView* GetDepthStencil();
};
