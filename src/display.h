#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "platecommon.h"
namespace PLATE
{
	class Display
	{
		public:
			Display(Plate * p, int w, int h, const char * title);

			void resize(int w, int h);
			void setTitle(const char * title);
			void render(void);
			Plate * getPlate(void);
			SDL_Window * getWindow(void);
			SDL_Renderer * getRenderer(void);
			void handleKey(SDL_KeyboardEvent k);
			bool GLok(const char* context="Display::GLok(unspecified)",bool term_on_err=true);

			~Display(void);

		private:
			Vec2 scroll;
			Vec2 speed;
			Plate * plate=NULL;

			SDL_Window * win=NULL;
			SDL_Renderer * rrr=NULL;
			SDL_GLContext glctx;
			int width,height;

			ColorTileset * ctset;
			TileLayer * tl;
			TileLayer * tl2;

			void resetGL(void);
			void resizeGL(void);
			void clearGL(void);
	};
};
#endif //__DISPLAY_H__
