#pragma once
#define NOMINMAX

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>
#include <string>
#include <vector>

#include <DirectXCollision.h>
#include "Terrain.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"

using namespace DirectX;

struct TransformationMatrices
{
	XMMATRIX worldmatrix;
	XMMATRIX viewmatrix;
	XMMATRIX projectionmatrix;
};

struct Light
{
	XMFLOAT4 diffuseColour;
	XMFLOAT3 pos;
	FLOAT padding;
};

class Engine
{
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	D3D11_VIEWPORT defaultviewport;
	ID3D11RenderTargetView* backbuffer;
	ID3D11DepthStencilView* depthstencilview;
	ID3D11DepthStencilState* defaultstencilstate, *nozstencilstate;

	ID3D11PixelShader *pixelshader;
	ID3D11VertexShader *vertexshader;

	ID3D11RasterizerState* clocklwise, *counterclockwise;

	UINT stride, offset;

	ID3DBlob* blobpixelvanilla, *blobvertexvanilla;

	ID3D11InputLayout* inputlayout;

	TransformationMatrices *transform;

	const FLOAT clearcolour[4];

	Mouse mouse;
	Keyboard keyboard;

	HWND windowhandle;

	std::vector<IDrawable*> models;

	Camera camera;
	Light* light;
	D3D11_MAPPED_SUBRESOURCE lightresouce, transformresource;

	ID3D11Buffer* lightbuffer, *matrixbuffer;

	VOID VanillaRender();
	VOID CreateRasterizerStates();
	VOID CompileShaders();
	VOID LoadDrawables();

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
	VOID Update();
};
