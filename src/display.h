#ifndef __DISPLAY_H__
#define __DISPLAY_H__
#include "platecommon.h"
namespace PLATE
{
	class Display
	{
		public:
			void Display(int w, int h, const char * title);

			void resize(int w, int h)
			void setTitle(const char * title);

			void ~Display(void);

		private:
			SDL_Window * win;
			int w,h;

	}
}
#endif //__DISPLAY_H__
