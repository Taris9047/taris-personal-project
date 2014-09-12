#ifndef WINSIMPLECLASS_CPP
#define WINSIMPLECLASS_CPP

#include "WinSimpleClass.hpp"
#include "Control.hpp"

WinSimpleClass::WinSimpleClass(int resId, HINSTANCE hInst)
	: _hInstance(hInst)
{
	ResString resStr(hInst, resId);
	_name = resStr;
}

HWND WinSimpleClass::GetRunningWindow()
{
	HWND hwnd = ::FindWindow(GetName(), 0);
	if (::IsWindow(hwnd))
	{
		HWND hwndPopup = ::GetLastActivePopup(hwnd);
		if (::IsWindow(hwndPopup))
			hwnd = hwndPopup;
	}
	else
		hwnd = 0;

	return hwnd;
}



#endif