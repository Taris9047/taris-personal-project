#include <Windows.h>
#include "TopWinClass.hpp"
#include "TopWinMaker.hpp"
#include "Control.hpp"
#include "resource.h"
#include <new.h>

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
	char * cmdParam, int cmdShow)
{
	// Using exceptions helps debugging the program.
	try {
		// Creating top window class
		TopWinClass topWinClass(ID_MAIN, hInst, MainWndProc);
		// If the same window runs, highlight it.
		HWND hwndOther = topWinClass.GetRunningWindow();

		if (hwndOther != 0) {
			::SetForegroundWindow(hwndOther);
			if (::IsIconic(hwndOther)) {
				::ShowWindow(hwndOther, SW_RESTORE);
			}
			return 0;
		}
		topWinClass.Register();

		// Create top window
		ResString caption(hInst, ID_CAPTION);
		TopWinMaker topWin(topWinClass, caption);
		topWin.Create();
		topWin.Show(cmdShow);

		// the Main message loop
		MSG msg;
		int status;
		while ((status = ::GetMessage(&msg, 0, 0, 0)) != 0) {
			if (status == -1) {
				return -1;
			}
			::TranslateMessage(&msg);
			::DispatchMessage(&msg);
		}

		return msg.wParam;
	}
	catch (WinException e)
	{
		WCHAR buf[50];
		wsprintf(buf, L"%s, Error %d", e.GetMessage(), e.GetError());
		::MessageBox(0, buf, L"Exception", MB_ICONEXCLAMATION | MB_OK);
	}
	catch (...)
	{
		::MessageBox(0, L"Unknown", L"Exception",
			MB_ICONEXCLAMATION | MB_OK);
	}

	return 0;
}