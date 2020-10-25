/******************************************************/
/*             Whacky Engine made by:                 */
/*    Emil Fransson, Oscar Milstein & Ove Ødegård     */
/******************************************************/

#define DEBUG

#include "Engine.hpp"
#include "Window.hpp"

INT APIENTRY wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPWSTR lpCmdLine, _In_ INT nShowCmd)
{
   CoInitialize(nullptr);
    Window window = Window(L"Demo engine");
    if (!window.GetHandle())
       return 1;
    Engine engine(window.GetHandle());

    ShowWindow(window.GetHandle(), nShowCmd);
    
    MSG msg = { };

    while (engine.Run())
    {
       // Run the message loop.
       while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
       {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
          if (msg.message == WM_QUIT)
             return 0;
       }
       
       // If no messages in message queue, do DirextX stuff...
       engine.Update();
    }

    return 0;
}