#ifndef TOPWINCLASS_CPP
#define TOPWINCLASS_CPP

#include "TopWinClass.hpp"

TopWinClass::TopWinClass(int resId,
	HINSTANCE hInst, WNDPROC wndProc)
	: WinClass(resId, hInst, wndProc)
{
	SetResIcons(resId);
	_class.lpszMenuName = MAKEINTRESOURCE(resId);
}


#endif