#ifndef WINCLASS_HPP
#define WINCLASS_HPP

#include <Windows.h>

LRESULT CALLBACK WindowProcedure
	(HWND hwnd, unsigned int message, WPARAM wParam, LPARAM lParam);

class WinClass
{
public:
	WinClass(WNDPROC winProc, LPCWSTR className, HINSTANCE hInst);

	void Register()
	{
		::RegisterClass(&_class);
	}

private:
	WNDCLASS _class;
};

#endif