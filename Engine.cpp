#include "Engine.hpp"

Engine* Engine::instance{ nullptr };

Engine::Engine(/*HINSTANCE hInstance, LPCWSTR wspTitle*/) /*: wnd(hInstance, wspTitle)*/
{
	/*DXGI_SWAP_CHAIN_DESC swap_chain_descr = { 0 };
	swap_chain_descr.BufferDesc.RefreshRate.Numerator = 0;
	swap_chain_descr.BufferDesc.RefreshRate.Denominator = 1;
	swap_chain_descr.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swap_chain_descr.SampleDesc.Count = 1;
	swap_chain_descr.SampleDesc.Quality = 0;
	swap_chain_descr.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swap_chain_descr.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swap_chain_descr.BufferCount = 2;
	swap_chain_descr.OutputWindow = wnd.GetHandle();
	swap_chain_descr.Windowed = true;*/


}

Engine::~Engine()
{
}

Engine *Engine::getInstance()
{
    if (!instance)
        instance = new Engine;
    return instance;
}
