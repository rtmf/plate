#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "platecommon.h"
namespace PLATE
{
	class Display
	{
		public:
			Display(Plate * p, int w, int h, const char * title);

			void setSize(int w, int h);
			void setTitle(const char * title);
			void render(void);
			Plate * getPlate(void);
			SDL_Window * getWindow(void);
			SDL_Renderer * getRenderer(void);
			void handleKey(SDL_KeyboardEvent k);
			bool GLok(const char* context="Display::GLok(unspecified)",bool term_on_err=true);

			~Display(void);


			GLuint vs,fs,sp;
			GLuint spaPosition, spaTexture;
			GLuint spuViewportSize,spuInverseSpriteTextureSize,spuTileSize,
				spuInverseTileSize,spuSprites,spuTiles,spuViewOffset,
				spuInverseTileTextureSize;

		private:
			Vec2 scroll;
			Vec2 speed;
			Plate * plate=NULL;

			SDL_Window * win=NULL;
			SDL_Renderer * rrr=NULL;
			SDL_GLContext glctx;
			int width,height;

			ColorTileset * ctset;
			TextureTileset * ttset;
			TileLayer * tl;
			TileLayer * tl2;
			Texture * tex;

			void resetGL(void);
			void orthoGL(void);
			void clearGL(void);


			float viewportSize[2];

			GLuint vbo;
			float verts[24]={
            //x  y  u  v
             0,  1, 0, 1,
             1,  1, 1, 1,
             1,  0, 1, 0,

             0,  1, 0, 1,
             1,  0, 1, 0,
             0,  0, 0, 0
			};
	};
};
#endif //__DISPLAY_H__
