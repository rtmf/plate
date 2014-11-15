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
	for(std::vector<std::string>::iterator search=searchPath.begin();search!=searchPath.end();++search)
	{
		path=*search+name;
		std::cout<<"Attempting to load:"<<path<<std::endl;
		tex=IMG_LoadTexture(dpy->getRenderer(),path.c_str());
		if (tex!=NULL) break;
	}
	if (tex==NULL)
		dpy->getPlate()->fatalError("Texture::Texture","failed to load texture");
	SDL_QueryTexture(tex,&format,&access,&w,&h);
	if (SDL_ISPIXELFORMAT_ALPHA(format)) std::cout<<"Texture has alpha!"<<std::endl;
}
void Texture::Bind(float * texw, float * texh)
{
	SDL_GL_BindTexture(tex,texw,texh);
}
void Texture::Unbind(void)
{
	SDL_GL_UnbindTexture(tex);
}
Texture::~Texture(void)
{
	SDL_DestroyTexture(tex);
}
