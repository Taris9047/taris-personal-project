#ifndef WINMAKER_HPP
#define WINMAKER_HPP

#include <Windows.h>

class WinMaker
{
public:
	WinMaker() : _hwnd(0) {}
	WinMaker(LPCWSTR caption,
		LPCWSTR className,
		HINSTANCE hInstance);
	void Show(int cmdShow)
	{
		::ShowWindow(_hwnd, cmdShow);
		::UpdateWindow(_hwnd);
	}
protected:
	HWND _hwnd;
};

#endif