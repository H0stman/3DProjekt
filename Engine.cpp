#include "Engine.hpp"

Engine* Engine::instance = nullptr;

Engine::Engine() : window(L"3D Engine"), mouse(), keyboard()
{
	DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
	swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_descr.SampleDesc.Count = 1;
	swap_chain_descr.SampleDesc.Quality = 0;
	swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_descr.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_descr.BufferCount = 2;
	swap_chain_descr.OutputWindow = window.GetHandle();
	swap_chain_descr.Windowed = true;

	D3D_FEATURE_LEVEL feature_level[] = { D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };

	UINT flags = D3D11_CREATE_DEVICE_SINGLETHREADED;
#if defined(DEBUG) || defined(_DEBUG)
	flags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, flags, nullptr, 0, D3D11_SDK_VERSION, &swap_chain_descr, &swapchain, &device, feature_level, &context);

	mouse.SetWindow(window.GetHandle());
	mouse.SetMode(Mouse::MODE_RELATIVE);
	
}

Engine::~Engine()
{
	device->Release();
	context->Release();
	swapchain->Release();
}

Engine *Engine::GetInstance()
{
    if (!instance)
        instance = new Engine();
    return instance;
}

void Engine::Release()
{
	this->~Engine();
}


ID3D11Device* Engine::GetDevice()
{
	return device;
}

ID3D11DeviceContext* Engine::GetContext()
{
	return context;
}


void Engine::UpdateCameraPosition()
{
	OutputDebugStringW((std::to_wstring(mouse.GetState().x) + L'\n').c_str());
	//OutputDebugStringW(std::to_wstring(L'\n').c_str());
}
