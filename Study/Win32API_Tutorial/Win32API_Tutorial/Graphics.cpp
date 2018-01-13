#ifndef GRAPHICS_CPP
#define GRAPHICS_CPP

#include "Graphics.hpp"

void View::Paint(Canvas &canvas, Model &model)
{
	canvas.Text(12, 1, model.GetText(), model.GetLen());
	canvas.Line(10, 0, 10, _cy);
}

#endif