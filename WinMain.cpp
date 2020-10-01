#define DEBUG

#include "Engine.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
    Engine* engine = Engine::GetInstance();

    if (!engine->window.GetHandle())
        return 0;

    ShowWindow(engine->window.GetHandle(), nShowCmd);
   
    // Run the message loop.
    MSG msg = { };
    while (GetMessage(&msg, nullptr, 0, 0))
    { 
        engine->UpdateCameraPosition();
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    engine->Release();
    return 0;
}