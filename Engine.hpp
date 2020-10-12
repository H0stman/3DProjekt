#pragma once
#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <DirectXMath.h>
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>
#include <string>
#include <vector>

#include "Texture.hpp"
#include "PixelShader.hpp"
#include "VertexShader.hpp"

using namespace DirectX;

class Engine
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	D3D11_VIEWPORT defaultviewport;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthstencilview;
	ID3D11DepthStencilState* defaultstencilstate, *nozstencilstate;

	PixelShader vanillapixelshader;
	VertexShader vanillavertexshader;

	const FLOAT clearcolour[4];

	Mouse mouse;
	Keyboard keyboard;

	HWND windowhandle;

public:

	BOOL Run();
	Engine() = default;
	Engine(const Engine &other) = default;
	Engine(HWND hndl);
	~Engine();

	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetContext();
	ID3D11RenderTargetView* GetBackbuffer();
	ID3D11DepthStencilView* GetDepthStencil();
	VOID VanillaRender();
	VOID Update();
};
