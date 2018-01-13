#ifndef TOPWINMAKER_CPP
#define TOPWINMAKER_CPP

#include <Windows.h>
#include "TopWinMaker.hpp"

TopWinMaker::TopWinMaker(WinClass &winClass, LPCWSTR caption)
	: WinMaker(winClass)
{
	_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
	_windowName = caption;
}

#endif