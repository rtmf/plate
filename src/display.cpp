#include "display.h"
using namespace PLATE;
void Display::Display(int w, int h, const char * title)
{
	win=SDL_CreateWindow(title,SDL_WINDOWPOS_CENTERED,
			SL_WINDOWPOS_CENTERED,w,h,SDL_WINDOW_OPENGL);
	this->w=w;
	this->h=h;
}

void Display::resize(int w, int h)
{
	if (win)
		SDL_DestroyWindow(win);
}

void Display::~Display(void)
{
}
