#ifndef CONTROL_CPP
#define CONTROL_CPP

#include "Control.hpp"
#include "resource.h"


Controller::Controller(HWND hwnd, CREATESTRUCT* pCreate)
	: _hwnd(hwnd), _model(L"Generic")
{
}

Controller::~Controller()
{
	::PostQuitMessage(0);
}

void Controller::Size(int cx, int cy)
{
	_view.SetSize(cx, cy);
}

void Controller::Paint()
{
	PaintCanvas canvas(_hwnd);
	_view.Paint(canvas, _model);
}

void Controller::Command(int cmd)
{
	switch (cmd)
	{
	case IDM_EXIT:
			::SendMessage(_hwnd, WM_CLOSE, 0, 0L);
			break;
		case IDM_HELP:
			::MessageBox(_hwnd, L"Go figure!",
				L"Generic", MB_ICONINFORMATION | MB_OK);
			break;
		case IDM_ABOUT:
			{
			HINSTANCE hInst = WinGetLong<HINSTANCE>(_hwnd, GWL_HINSTANCE);
				::DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUT),
					_hwnd,
					AboutDlgProc);
			}
			break;
	}
}






int NewHandler(size_t size)
{
	throw WinException(L"Out of Memory!!!");
	return 0;
}

ResString::ResString(HINSTANCE hInst, int resId)
{
	if (!::LoadString(hInst, resId, _buf, MAX_RESSTRING + 1))
		throw WinException(L"Load String failed!!");
}

BOOL CALLBACK AboutDlgProc
(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_INITDIALOG:
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDOK:
		case IDCANCEL:
			::EndDialog(hwnd, 0);
			return TRUE;
		}
		break;
	}
	return FALSE;
}

// Window Procedure

LRESULT CALLBACK MainWndProc
(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	// Pointer to Controller is stored in Window
	Controller * pCtrl = WinGetLong<Controller *>(hwnd);

	switch (message)
	{
	case WM_CREATE:
		try
		{
			pCtrl = new Controller(hwnd, reinterpret_cast<CREATESTRUCT *> (lParam));
			// Store pointer to Controller inside Window
			WinSetLong<Controller *>(hwnd, pCtrl);
		}
		catch (WinException e)
		{
			::MessageBox(hwnd, e.GetMessage(), L"Initialization",
				MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}
		catch (...)
		{
			::MessageBox(hwnd, L"Unknown Error", L"Initialization",
				MB_ICONEXCLAMATION | MB_OK);
			return -1;
		}
		return 0;
	case WM_SIZE:
		pCtrl->Size(LOWORD(lParam), HIWORD(lParam));
		return 0;
	case WM_PAINT:
		pCtrl->Paint();
		return 0;
	case WM_COMMAND:
		pCtrl->Command(LOWORD(wParam));
		return 0;
	case WM_DESTROY:
		WinSetLong<Controller *>(hwnd, 0);
		delete pCtrl;
		return 0;
	}
	return ::DefWindowProc(hwnd, message, wParam, lParam);
}

#endif