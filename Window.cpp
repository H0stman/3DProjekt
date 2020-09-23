#include "Window.hpp"

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
            DestroyWindow(hwnd);
        else
            return 0;
    }
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

Window::Window(HINSTANCE hInstance, LPCWSTR wspTitle)
{
    wc = { };
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //Flags [Redraw on width/height change from resize/movement] See: https://msdn.microsoft.com/en-us/library/windows/desktop/ff729176(v=vs.85).aspx
    wc.lpfnWndProc = WindowProc; //Pointer to Window Proc function for handling messages from this window
    wc.cbClsExtra = 0; //# of extra bytes to allocate following the window-class structure. We are not currently using this.
    wc.cbWndExtra = 0; //# of extra bytes to allocate following the window instance. We are not currently using this.
    wc.hInstance = hInstance; //Handle to the instance that contains the Window Procedure
    wc.hIcon = NULL;   //Handle to the class icon. Must be a handle to an icon resource. We are not currently assigning an icon, so this is null.
    wc.hIconSm = NULL; //Handle to small icon for this class. We are not currently assigning an icon, so this is null.
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); //Default Cursor - If we leave this null, we have to explicitly set the cursor's shape each time it enters the window.
    wc.hbrBackground = NULL; //Handle to the class background brush for the window's background color - we will leave this blank for now and later set this to black. For stock brushes, see: https://msdn.microsoft.com/en-us/library/windows/desktop/dd144925(v=vs.85).aspx
    wc.lpszMenuName = NULL; //Pointer to a null terminated character string for the menu. We are not using a menu yet, so this will be NULL.
    wc.lpszClassName = CLASS_NAME; //Pointer to null terminated string of our class name for this window.
    wc.cbSize = sizeof(WNDCLASSEX); //Need to fill in the size of our struct for cbSize

    RegisterClassEx(&wc);
    hndl = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        wspTitle,                       // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window    
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
    );
}

HWND Window::GetHandle()
{
    return hndl;
}


