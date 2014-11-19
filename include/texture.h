#ifndef __TEXTURE_H__
#define __TEXTURE_H__
#include "platecommon.h"
namespace PLATE
{
	class Texture
	{
		public:
			Texture(Display * dpy, const char * basename);
			int getWidth(void);
			int getHeight(void);
			GLint getTextureName(void);
			~Texture(void);
		private:
			Display * d;
			GLuint tex;
			Uint32 format;
			int access;
			int w;
			int h;
	};
};
#endif //__TEXTURE_H__
