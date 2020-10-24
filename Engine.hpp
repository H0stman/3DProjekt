#pragma once
#define NOMINMAX

#include <d3d11_4.h>
#include <dxgi1_6.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#include <DirectXCollision.h>
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>
#include <string>
#include <vector>

#include <DirectXCollision.h>
#include "Terrain.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"
#include "PointLight.h"
#include "ShadowMap.h"
#include "QuadTree.hpp"
#include "Water.hpp"

#define QUADTARGET 2
#define DEFERREDTARGET 3

using namespace DirectX;

struct TransformationMatrices
{
	XMMATRIX worldmatrix;
	XMMATRIX viewmatrix;
	XMMATRIX projectionmatrix;
	XMFLOAT3 camerapos;
};

struct PhongLight_ConstantBuffer_PS
{
	DirectX::XMFLOAT3 ambientColor;
	float padding1;
	DirectX::XMFLOAT3 diffuseColor;
	float padding2;
	DirectX::XMFLOAT3 diffuseLightPosition;
	float diffuseLightIntensity;
	DirectX::XMFLOAT3 padding;
	float ambientLightIntensity;
	float attenuationConstant;
	float attenuationLinear;
	float attenuationQuadratic;
	float specularIntensity;
};

struct Particle
{
	XMFLOAT3 pos;
};

class Engine
{
private:
	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGISwapChain* swapchain;
	D3D11_VIEWPORT defaultviewport;
	ID3D11RenderTargetView* backbuffer;
	Texture* rendertexture, *blurtarget;
	ID3D11DepthStencilView* depthstencilview;
	ID3D11DepthStencilState* defaultstencilstate, *nozstencilstate;

	ID3D11PixelShader *pixelshader, *pixelshader2D, *pixelshadergbuf, *pixelshaderlight;
	ID3D11VertexShader *vertexshader, *vertexshader2D, *vertexshadertess, *vertexshaderdeferred, *vertexshadershadow, * vertexshaderparticle;
	ID3D11ComputeShader* csblurshader;
	ID3D11HullShader* hullshader;
	ID3D11DomainShader* domainshader;
	ID3D11GeometryShader* geometryshaderparticle;
	ID3D11RasterizerState* clockwise, *counterclockwise;

	UINT stride, offset;

	ID3DBlob* blobpixelvanilla, *blobpixel2D, *blobpixelgbuf, *blobpixellight,*blobvertexvanilla, *blobvertextess, *blobvertex2D, *blobvertexDeferred, *blobvertexshadow, *blobcsblur, *blobhullshader, *blobdomainshader, * blobgeometryparticle, * blobvertexparticle;

	ID3D11InputLayout* inputlayout;

	TransformationMatrices *transform;

	const FLOAT clearcolour[4];

	Mouse mouse;
	Keyboard keyboard;

	HWND windowhandle;

	std::vector<IDrawable*> models;
	QuadTree quadtree;
	std::vector<Particle> particlepositions;

	Camera camera;
	PointLight pointLight;
	D3D11_MAPPED_SUBRESOURCE lightresource, transformresource;
	Water* water;
	ID3D11SamplerState* texturesampler;

	ID3D11ShaderResourceView* particleview;

	ID3D11Buffer* lightbuffer, *matrixbuffer, *render2Dquad, *particlebuffer, *indirectargs;

	static constexpr unsigned int nrOfBuffers{ 3u };
	Texture* gbufNormal;
	Texture* gbufDiffuse;
	Texture* gbufPosition;
	//Texture* gbufLightCS;

	PhongLight_ConstantBuffer_PS* phongLight;
	ShadowMap shadowMap;
private:
	VOID InitializeDeferredRendererResources(UINT width, UINT height);
	VOID ReadyGeometryPassResources();
	VOID ReadyLightPassResources();
	VOID ReadyShadowPass();
	VOID DeferredRenderer();
	VOID DeferredGeometryPass();
	VOID DeferredLightPass();
	VOID ShadowPass();
	VOID ReadyTessellation();
	VOID Render2D(Texture* tex);
	VOID CreateRasterizerStates();
	VOID CreateParticles();
	VOID CompileShaders();
	VOID LoadDrawables();
	VOID DrawParticles();
	VOID SetRenderTargets(UINT target);
	VOID ClearRenderTargets(UINT target);
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
