#include "Window.hpp"
#include <DirectXTK\Mouse.h>
#include <DirectXTK\Keyboard.h>

Window::Window(LPCWSTR wspTitle)
{
    wc = { 0 };
    wc.style = CS_HREDRAW | CS_VREDRAW;                  //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
    wc.lpfnWndProc = WindowProc;                         //Pointer to Window Proc function for handling messages from this window
    wc.cbClsExtra = 0;                                   //# of extra bytes to allocate following the window-class structure. We are not currently using this.
    wc.cbWndExtra = 0;                                   //# of extra bytes to allocate following the window instance. We are not currently using this.
    wc.hInstance = (HINSTANCE)GetModuleHandle(nullptr);  //Handle to the instance that contains the Window Procedure
    wc.hIcon = nullptr;                                  //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
    wc.hIconSm = nullptr;                                //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
    wc.hCursor = LoadCursor(nullptr, IDC_ARROW);         //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
    wc.hbrBackground = (HBRUSH)COLOR_WINDOW;             //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
    wc.lpszMenuName = nullptr;                           //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
    wc.lpszClassName = CLASS_NAME;                       //Pointer to null terminated string of our class name for this window.
    wc.cbSize = sizeof(WNDCLASSEX);                      //Need to fill in the size of our struct for cbSize

    RegisterClassEx(&wc);
    hndl = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        wspTitle,                       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        nullptr,       // Parent window    
        nullptr,       // Menu
        (HINSTANCE)GetModuleHandle(nullptr),  // Instance handle
        nullptr        // Additional application data
    );

    RECT rect;
    if (GetWindowRect(hndl, &rect))
    {
       width = rect.right - rect.left;
       height = rect.bottom - rect.top;
    }
    else
    {
       PWSTR messageBuffer;
       DWORD size = FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_ALLOCATE_BUFFER, nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (PWSTR)&messageBuffer, 0, nullptr);
       OutputDebugStringW(messageBuffer);
    }

}

LRESULT Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW));
        EndPaint(hwnd, &ps);
        break;
    }
    case WM_CLOSE:
    {
       if (MessageBox(hwnd, L"Quit?", L"Exit", MB_YESNO) == IDYES)
       {
          DestroyWindow(hwnd);
       }
        else
            return 0;
    }
    case WM_ACTIVATEAPP:
       DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
       DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
       break;
    case WM_INPUT:
    case WM_MOUSEMOVE:
    case WM_LBUTTONDOWN:
    case WM_LBUTTONUP:
    case WM_RBUTTONDOWN:
    case WM_RBUTTONUP:
    case WM_MBUTTONDOWN:
    case WM_MBUTTONUP:
    case WM_MOUSEWHEEL:
    case WM_XBUTTONDOWN:
    case WM_XBUTTONUP:
    case WM_MOUSEHOVER:
       DirectX::Mouse::ProcessMessage(uMsg, wParam, lParam);
       break;

    case WM_KEYDOWN:
    case WM_KEYUP:
    case WM_SYSKEYUP:
       DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
       break;

    case WM_SYSKEYDOWN:
       DirectX::Keyboard::ProcessMessage(uMsg, wParam, lParam);
       if (wParam == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
       {
          
       }
       break;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

HWND Window::GetHandle()
{
    return hndl;
}

INT Window::GetWidth()
{
   return width;
}

INT Window::GetHeight()
{
   return height;
}