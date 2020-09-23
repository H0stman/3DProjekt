#include "Engine.hpp"
#include "Window.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
    Window wnd = Window(hInstance, L"3D Project engine");

    if (!wnd.GetHandle())
        return 0;

    ShowWindow(wnd.GetHandle(), nShowCmd);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}