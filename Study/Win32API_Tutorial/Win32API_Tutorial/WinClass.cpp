#ifndef WINCLASS_CPP
#define WINCLASS_CPP

#include "WinClass.hpp"
#include "Control.hpp"

WinClass::WinClass(LPCWSTR className,
	HINSTANCE hInst, WNDPROC wndProc)
	: WinSimpleClass(className, hInst)
{
	_class.lpfnWndProc = wndProc;
	SetDefaults();
}

WinClass::WinClass(int resId,
	HINSTANCE hInst, WNDPROC wndProc)
	: WinSimpleClass(resId, hInst)
{
	_class.lpfnWndProc = wndProc;
	SetDefaults();
}

void WinClass::SetDefaults()
{
	_class.cbSize = sizeof(WNDCLASSEX);
	_class.style = 0;
	_class.lpszClassName = GetName();
	_class.hInstance = GetInstance();
	_class.hIcon = 0;
	_class.hIconSm = 0;
	_class.lpszMenuName = 0;
	_class.cbClsExtra = 0;
	_class.cbWndExtra = 0;
	_class.hbrBackground = reinterpret_cast<HBRUSH> (COLOR_WINDOW + 1);
	_class.hCursor = ::LoadCursor(0, IDC_ARROW);
}

void WinClass::SetResIcons(int resId)
{
	_class.hIcon = reinterpret_cast<HICON> (
		::LoadImage(
		_class.hInstance,
		MAKEINTRESOURCE(resId),
		IMAGE_ICON,
		::GetSystemMetrics(SM_CXICON),
		::GetSystemMetrics(SM_CYICON),
		0));

	// Small icon can be loaded from the same resource
	_class.hIconSm = reinterpret_cast <HICON> (
		::LoadImage(
		_class.hInstance,
		MAKEINTRESOURCE(resId),
		IMAGE_ICON,
		::GetSystemMetrics(SM_CXSMICON),
		::GetSystemMetrics(SM_CYSMICON),
		0));
}

void WinClass::Register()
{
	if (::RegisterClassEx(&_class) == 0)
		throw WinException(L"Internal error: RegisterClassEx failed.");
}




#endif