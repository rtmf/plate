#include "plate.h"
#include "display.h"
using namespace PLATE;
Plate::Plate(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING)!=0)
	{
		this->fatalSDLError("Plate::Plate");
	}
	d = new Display(this,640,480,"PLATE Demo");
	isRunning=true;
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
		d->render();
	}
	return 0;
}
Plate::~Plate(void)
{
	SDL_Quit();
}
