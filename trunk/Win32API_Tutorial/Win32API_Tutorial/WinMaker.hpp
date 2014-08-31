#ifndef WINMAKER_HPP
#define WINMAKER_HPP

#include <Windows.h>
#include "WinClass.hpp"

class WinMaker
{
public:
	WinMaker(WinClass &winClass);
	operator HWND () { return _hwnd; }
	void AddCaption(LPCWSTR caption)
	{
		_windowName = caption;
	}
	void AddSysMenu() { _style |= WS_SYSMENU; }
	void AddVScrollBar() { _style |= WS_VSCROLL; }
	void AddHScrollBar() { _style |= WS_HSCROLL; }
	void Create();
	void Show(int nCmdShow = SW_SHOWNORMAL);

protected:
	WinClass &_class;
	HWND _hwnd;

	DWORD _exStyle;			// extended window style
	LPCWSTR _windowName;	// pointer to window name
	DWORD _style;			// window style
	int _x;					// horizontal position of window
	int _y;					// vertical position of window
	int _width;				// window width
	int _height;			// window height
	HWND _hWndParent;		// handle to parent or owner window
	HMENU _hMenu;			// handle to menu, or child-window
	void *_data;			// pointer to window-creatoin data
};

#endif
