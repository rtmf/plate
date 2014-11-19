#include "plate.h"
#include "display.h"
#include "oglconsole.h"
using namespace PLATE;
Plate::Plate(void)
{
	char * temppath;
	std::string basepath;
	std::string prefpath;

	if (SDL_Init(SDL_INIT_EVERYTHING)!=0)
	{
		this->fatalSDLError("Plate::Plate");
	}

	// XXX this is ugly, consider boost::filesystem & co

	temppath=SDL_GetBasePath();
	basepath=std::string(temppath);
	SDL_free(temppath);
	pathsep=basepath.substr(basepath.length()-1,1);
	temppath=SDL_GetPrefPath("Concordia","PLATE Demo");
	prefpath=std::string(temppath);
	SDL_free(temppath);
	textureSearchPath.push_back(prefpath+std::string("override")+
		pathsep+std::string("textures")+pathsep);
	textureSearchPath.push_back(basepath+std::string("textures")+pathsep);
	textureSearchPath.push_back(basepath+std::string("..")+
		pathsep+std::string("textures")+pathsep);
	textureSearchPath.push_back(basepath);
	textureSearchPath.push_back(std::string(".")+pathsep+std::string("textures")+pathsep);
	textureSearchPath.push_back(std::string(".")+pathsep);


	d = new Display(this,640,480,"PLATE Demo");
	isRunning=true;
}

std::vector<std::string> Plate::getTextureSearchPath(void)
{
	return textureSearchPath;
}
std::string Plate::getPathSep(void)
{
	return pathsep;
}

Display * Plate::getDisplay(void)
{
	return d;
}
void Plate::fatalSDLError(const char * context)
{
	std::cerr<<"[PLATE] @{"<<context<<"} %%SDL_ERROR{\""<<SDL_GetError()<<"\"} !!FATAL"<<std::endl;
	SDL_Quit();
	exit(PLATE_RETURN_SDL_FATAL);
}
void Plate::fatalError(const char * context, const char * err)
{
	std::cerr<<"[PLATE] @{"<<context<<"} %%ERROR{\""<<err<<"\"} !!FATAL"<<std::endl;
	SDL_Quit();
	exit(PLATE_RETURN_OTHER_FATAL);
}
void Plate::logError(const char * context, const char * err)
{
	std::cerr<<"[PLATE] @{"<<context<<"} %%ERROR{\""<<err<<"\"}"<<std::endl;
}


int Plate::run(int argc, char ** argv)
{
	//TODO move event handling entirely
	SDL_Event e;
	//main event loop
	while(isRunning)
	{
		while(SDL_PollEvent(&e))
		{
			if (!OGLCONSOLE_SDLEvent(&e))
			{
				switch(e.type)
				{
					case SDL_QUIT:
						isRunning=false;
						break;
					case SDL_KEYDOWN:
					case SDL_KEYUP:
						d->handleKey(e.key);
						break;
					default:
						break;
				}
			}
		}
		d->render();
	}
	return 0;
}
Plate::~Plate(void)
{
	SDL_Quit();
}
