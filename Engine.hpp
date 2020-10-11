#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>
#include <string>
#include <vector>

#include "Window.hpp"
#include "Texture.hpp"

using namespace DirectX;

class Engine
{
	static Engine* instance;
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	D3D11_VIEWPORT defaultviewport;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthstencilview;
	ID3D11DepthStencilState* defaultstencilstate;
	ID3D11DepthStencilState* nozstencilstate;

	ID3D11Texture2D* pBackBuffer, *pDepthStencilBuffer; //TODO: Change name according to convention.

	Mouse mouse;
	Keyboard keyboard;

	Engine();

public:

	Window window;

	static Engine *GetInstance();
	BOOL Run();
	~Engine();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	ID3D11RenderTargetView* GetBackbuffer();
	ID3D11DepthStencilView* GetDepthStencil();
	VOID Update();
};
