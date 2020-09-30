#define DEBUG

#include "Engine.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
    Engine* engine = Engine::GetInstance();

    if (!engine->wnd.GetHandle())
        return 0;

    ShowWindow(engine->wnd.GetHandle(), nShowCmd);
   
    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}