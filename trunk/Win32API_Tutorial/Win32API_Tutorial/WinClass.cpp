#ifndef WINCLASS_CPP
#define WINCLASS_CPP

#include "WinClass.hpp"

WinClass::WinClass
(WNDPROC winProc, LPCWSTR className, HINSTANCE hInst)
{
	_class.style = 0;
	_class.lpfnWndProc = winProc; // window procedure: mandatory
	_class.cbClsExtra = 0;
	_class.cbWndExtra = 0;
	_class.hInstance = hInst;         // owner of the class: mandatory
	_class.hIcon = 0;
	_class.hCursor = ::LoadCursor(0, IDC_ARROW); // optional
	_class.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // optional
	_class.lpszMenuName = 0;
	_class.lpszClassName = className; // mandatory
}

#endif