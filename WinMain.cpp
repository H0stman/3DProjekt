#define DEBUG

#include "Engine.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
    Engine* engine = Engine::GetInstance();

    if (!engine->window.GetHandle())
        return 1;

    ShowWindow(engine->window.GetHandle(), nShowCmd);
   
    
    MSG msg = { };

    while (engine->Run())
    {
       // Run the message loop.
       while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
       {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
       }

       // If no messages in message queue, do DirextX stuff...

       engine->UpdateCameraPosition();
    }

    delete engine;
    return 0;
}