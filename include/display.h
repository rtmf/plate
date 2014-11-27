#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "platecommon.h"
#include "shader.h"
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


			ShaderProgram* shaderProgram;
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

			/* Queries all ShaderPrograms used by Display for their vertex attribute
			 * and uniform locations, and assigns them to their corresponding member
			 * variables. This should be done after shaders are loaded initially, or
			 * after a call to ShaderProgram::rebuild().
			 */
			void getShaderVariableLocations();


			float viewportSize[2];

			static void staticGLInitialization();
			static GLuint vbo;
	};
};
#endif //__DISPLAY_H__
