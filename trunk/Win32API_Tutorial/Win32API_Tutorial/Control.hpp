#ifndef CONTROL_HPP
#define CONTROL_HPP

#include <Windows.h>
#include "Model.hpp"
#include "Graphics.hpp"

// Getting and Setting WindowLong: default is GWL_USERDATA

template <class T>
inline T WinGetLong(HWND hwnd, int which = GWL_USERDATA)
{
	return reinterpret_cast<T> (::GetWindowLong(hwnd, which));
}

template <class T>
inline void WinSetLong(HWND hwnd, T value, int which = GWL_USERDATA)
{
	::SetWindowLong(hwnd, which, reinterpret_cast<long> (value));
}

class Controller
{
public:
	Controller(HWND hwnd, CREATESTRUCT* pCreate);
	~Controller();
	void Size(int x, int y);
	void Paint();
	void Command(int cmd);
private:
	HWND _hwnd;
	Model _model;
	View _view;
};


class WinException
{
public:
	WinException(LPWSTR msg)
		: _err(::GetLastError()), _msg(msg)
	{}
	DWORD GetError() const { return _err; }
	LPCWSTR GetMessage() const { return _msg; }
private:
	DWORD _err;
	LPWSTR _msg;
};

class ResString
{
	enum {MAX_RESSTRING = 255};
public:
	ResString(HINSTANCE hInst, int resId);
	operator LPWSTR () { return _buf; }
private:
	LPWSTR _buf;
};

LRESULT CALLBACK MainWndProc
(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

BOOL CALLBACK AboutDlgProc
(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

#endif