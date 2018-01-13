#ifndef GRAPHICS_HPP
#define GRAPHICS_HPP

#include<Windows.h>
#include "Model.hpp"

class Canvas
{
public:
	operator HDC () { return _hdc; }

	void Line(int x1, int y1, int x2, int y2)
	{
		::MoveToEx(_hdc, x1, y1, 0);
		::LineTo(_hdc, x2, y2);
	}

	void Text(int x, int y, LPCWSTR buf, int cBuf)
	{
		::TextOut(_hdc, x, y, buf, cBuf);
	}

	void Char(int x, int y, WCHAR c)
	{
		::TextOut(_hdc, x, y, &c, 1);
	}

protected:
	Canvas(HDC hdc) : _hdc(hdc) {}

	HDC _hdc;
};


class View
{
public:
	void SetSize(int cxNew, int cyNew)
	{
		_cx = cxNew;
		_cy = cyNew;
	}

	void Paint(Canvas &canvas, Model &model);

protected:
	int _cx;
	int _cy;
};


class PaintCanvas : public Canvas
{
public:
	// Constructor obtains the DC
	PaintCanvas(HWND hwnd)
		: Canvas(::BeginPaint(hwnd, &_paint)),
		_hwnd(hwnd)
	{}

	// Destructor releases the DC
	~PaintCanvas()
	{
		::EndPaint(_hwnd, &_paint);
	}

protected:

	PAINTSTRUCT _paint;
	HWND        _hwnd;
};



#endif