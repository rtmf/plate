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

			~Display(void);

		private:
			Plate * plate=NULL;

			SDL_Window * win=NULL;
			SDL_Renderer * rrr=NULL;
			int width,height;

			void resetGL(void);
			void resizeGL(void);
			void clearGL(void);
	};
};
#endif //__DISPLAY_H__
