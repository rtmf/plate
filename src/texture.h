#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "platecommon.h"
namespace PLATE
{
	class Texture
	{
		public:
			Texture(Display * dpy, const char * basename);
			void Bind(float * texw=NULL,float * texh=NULL);
			void Unbind(void);
			~Texture(void);
		private:
			Display * d;
			SDL_Texture * tex;
			Uint32 format;
			int access;
			int w;
			int h;
	};
};
#endif //__TEXTURE_H__
