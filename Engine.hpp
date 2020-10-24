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
#include "QuadTree.hpp"

using namespace DirectX;

struct TransformationMatrices
{
	XMMATRIX worldmatrix;
	XMMATRIX viewmatrix;
	XMMATRIX projectionmatrix;
	XMFLOAT3 camerapos;
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
	Texture* rendertexture, *blurtarget, *gbufcolor, *gbufnormals;
	ID3D11DepthStencilView* depthstencilview;
	ID3D11DepthStencilState* defaultstencilstate, *nozstencilstate;

	ID3D11PixelShader *pixelshader, *pixelshader2D;
	ID3D11VertexShader *vertexshader, *vertexshader2D, *vertexshadertess;
	ID3D11ComputeShader* csblurshader;
	ID3D11HullShader* hullshader;
	ID3D11DomainShader* domainshader;

	ID3D11RasterizerState* clockwise, *counterclockwise;

	UINT stride, offset;

	ID3DBlob* blobpixelvanilla, *blobpixel2D, *blobvertexvanilla, *blobvertextess, *blobvertex2D, *blobcsblur, *blobhullshader, *blobdomainshader;

	ID3D11InputLayout* inputlayout;

	TransformationMatrices *transform;

	const FLOAT clearcolour[4];

	Mouse mouse;
	Keyboard keyboard;

	HWND windowhandle;

	std::vector<IDrawable*> models;
	QuadTree quadtree;

	Camera camera;
	Light* light;
	D3D11_MAPPED_SUBRESOURCE lightresouce, transformresource;

	ID3D11SamplerState* texturesampler;

	ID3D11Buffer* lightbuffer, *matrixbuffer, *render2Dquad;

	VOID VanillaRender();
	VOID Deferred();
	VOID DeferredLightPass();
	VOID ShadowPass();
	VOID Tessellation();
	VOID Render2D(Texture* tex);
	VOID CreateRasterizerStates();
	VOID CompileShaders();
	VOID LoadDrawables();
	VOID SetRenderTargets(UINT target);
	VOID Blur(Texture* source, Texture* target);

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
