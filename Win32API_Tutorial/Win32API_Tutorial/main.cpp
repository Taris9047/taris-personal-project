#include <Windows.h>
#include "WinClass.hpp"
#include "WinMaker.hpp"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst,
	char * cmdParam, int cmdShow)
{
	LPCWSTR className = L"Winnie";

	WinClass winClass(WindowProcedure, className, hInst);
	winClass.Register();

	WinMaker win(L"Hello Windows!!", className, hInst);
	win.Show(cmdShow);

	MSG msg;
	int status;

	while ((status = ::GetMessage(&msg, 0, 0, 0)) != 0)
	{
		if (status == -1) {
			return -1;
		}
		::DispatchMessage(&msg);
	}

	return msg.wParam;
}