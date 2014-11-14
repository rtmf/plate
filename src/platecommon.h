#ifndef __PLATECOMMON_H__
#define __PLATECOMMON_H__
/*
	PLAtform Engine

	Requires:
		SDL2
		SDL2_Image
		OpenGL
*/
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <GL/glu.h>
#define PLATE_RETURN_SDL_FATAL 127
#define PLATE_RETURN_OTHER_FATAL 63
namespace PLATE
{
	class Display;
	class Plate;
	class Config;
	class Tileset;
	class ColorTileset;
	class TileLayer;
	class Texture;
}
#include "vec2.h"
#endif //__PLATECOMMON_H__
