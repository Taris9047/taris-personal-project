#ifndef WINCLASS_HPP
#define WINCLASS_HPP

#include <Windows.h>
#include "WinSimpleClass.hpp"

class WinClass: public WinSimpleClass
{
public:
	WinClass(LPCWSTR className, HINSTANCE hInst, WNDPROC wndProc);
	WinClass(int resId, HINSTANCE hInst, WNDPROC wndProc);

	void SetBgSysColor(int sysColor)
	{
		_class.hbrBackground = reinterpret_cast<HBRUSH> (sysColor + 1);
	}
	void SetResIcons(int resId);
	void Register();

protected:
	void SetDefaults();
	WNDCLASSEX _class;
};

#endif