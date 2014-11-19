#include "texture.h"
#include "plate.h"
#include "display.h"
#include <string>
#include <vector>

using namespace PLATE;

Texture::Texture(Display * dpy, const char * basename)
{
	std::string name(basename);
	std::string path;
	std::vector<std::string> searchPath=dpy->getPlate()->getTextureSearchPath();
	SDL_Surface * surf;
	for(std::vector<std::string>::iterator search=searchPath.begin();search!=searchPath.end();++search)
	{
		path=*search+name;
		std::cout<<"Attempting to load:"<<path<<std::endl;
		surf = IMG_Load(path.c_str());
		if (surf!=NULL) break;
	}
	if (surf==NULL)
		dpy->getPlate()->fatalError("Texture::Texture","failed to load texture");

	/* We now have an SDL_Surface with our texture. */
	/* Without this, the code gets more complex, but we can count on it, Scouts' honor. */
	if (surf->w * surf->format->BytesPerPixel != surf->pitch)
		dpy->getPlate()->fatalError("Texture::Texture","SDL_image produced an unsupported surface");
	/*Let's support 24-bit and 32-bit color for now. */
	GLenum format;
	switch (surf->format->BytesPerPixel)
	{
		default:
			dpy->getPlate()->fatalError("Texture::Texture","texture has unsupported color depth");
			*((char*)0) = 0;
		case 4:
			format = (surf->format->Rmask == 255) ? GL_RGBA : GL_BGRA;
			break;
		case 3:
			format = (surf->format->Rmask == 255) ? GL_RGB : GL_BGR;
			break;
	}
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, surf->w, surf->h, 0, format, GL_UNSIGNED_BYTE, surf->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	w = surf->w;
	h = surf->h;
	SDL_FreeSurface(surf);
}
int Texture::getWidth(void)
{
	return w;
}
int Texture::getHeight(void)
{
	return h;
}
GLint Texture::getTextureName(void)
{
	return tex;
}
Texture::~Texture(void)
{
	glDeleteTextures(1, &tex);
}
