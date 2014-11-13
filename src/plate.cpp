#include "plate.h"
using namespace PLATE;
void Plate::Plate(void)
{
	if (SDL_Init(SDL_INIT_EVERYTHING))
		
	d = new Display(this,640,480,"PLATE Demo");
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

void Plate::main(int argc, char ** argv)
{
	//main event loop
	while(isRunning)
	{
		d->render();
	}
	return 0;
}
void Plate::~Plate(void)
{
	SDL_Quit();
}
