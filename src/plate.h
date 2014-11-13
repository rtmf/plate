#ifndef __PLATE_H__
#define __PLATE_H__
#include "platecommon.h"
#include "display.h"
//#include "oglconsole.h"
//XXX not using this yet
#include <iostream>
namespace PLATE
{
	class Plate
	{
		public:
			void Plate(void);

			int main(int argc, char ** argv);
			void fatalSDLError(const char * context);
			void fatalError(const char * context);

			void ~Plate(void);
			
		private:
			Display * d=NULL;
			bool isRunning=true;
	}
}
#endif //__PLATE_H__
