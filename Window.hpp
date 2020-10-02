#pragma once
#include <Windows.h>
class Window
{
	LPCWSTR CLASS_NAME = L"3D Project Window Class";
	WNDCLASSEX wc;
	HWND hndl;
	INT width, height;

	Window& operator=(const Window&) = delete; // no self-assignments
public:
	
	Window(LPCWSTR wspTitle);
	HWND GetHandle();
	INT GetWidth();
	INT GetHeight();
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
};

