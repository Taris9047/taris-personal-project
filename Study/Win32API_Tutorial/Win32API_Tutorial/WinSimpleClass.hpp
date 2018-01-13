#ifndef WINSIMPLECLASS_HPP
#define WINSIMPLECLASS_HPP

#include <Windows.h>
#include <string>

class WinSimpleClass
{
public:
	WinSimpleClass(LPCWSTR name, HINSTANCE hInst)
		: _name(name), _hInstance(hInst)
	{}

	WinSimpleClass(int resId, HINSTANCE hInst);
	LPCWSTR GetName() const { return _name; }
	HINSTANCE GetInstance() const { return _hInstance; }

	HWND GetRunningWindow();

protected:
	HINSTANCE _hInstance;
	LPCWSTR _name;
};




#endif