#ifndef WINMAKER_CPP
#define WINMAKER_CPP

#include "WinMaker.hpp"

WinMaker::WinMaker(LPCWSTR caption,
	LPCWSTR className,
	HINSTANCE hInstance)
{
	_hwnd = ::CreateWindow(
		className,            // name of a registered window class
		caption,              // window caption
		WS_OVERLAPPEDWINDOW,  // window style
		CW_USEDEFAULT,        // x position
		CW_USEDEFAULT,        // y position
		CW_USEDEFAULT,        // witdh
		CW_USEDEFAULT,        // height
		0,                    // handle to parent window
		0,                    // handle to menu
		hInstance,            // application instance
		0);                   // window creation data
}


// Window Procedure called by Windows
LRESULT CALLBACK WindowProcedure
(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_DESTROY:
		::PostQuitMessage(0);
		return 0;

	}
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}


#endif