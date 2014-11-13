#include "plate.h"
using namespace PLATE;
void Plate::Plate(void)
{
	SDL_Init(SDL_INIT_EVERYTHING);
	d = new Display(640,480,"PLATE Demo");
}
void Plate::main(int argc, char ** argv)
{
	//main event loop
	while(true)
	{
		d->render();
	}
}
void Plate::~Plate(void)
{
	SDL_Quit();
}
