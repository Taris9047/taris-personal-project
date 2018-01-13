#ifndef TOPWINCLASS_HPP
#define TOPWINCLASS_HPP

#include "WinClass.hpp"

class TopWinClass : public WinClass
{
public:
	TopWinClass(int resId, HINSTANCE hInst, WNDPROC wndProc);
};

#endif