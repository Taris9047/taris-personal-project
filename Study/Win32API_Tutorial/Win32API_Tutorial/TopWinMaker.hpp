#ifndef TOPWINMAKER_HPP
#define TOPWINMAKER_HPP

#include "WinMaker.hpp"

class TopWinMaker : public WinMaker
{
public:
	TopWinMaker(WinClass &winClass, LPCWSTR caption);
};



#endif