#ifndef __PLATE_H__
#define __PLATE_H__
#include "platecommon.h"
//#include "oglconsole.h"
//XXX not using this yet
#include <iostream>
namespace PLATE
{
	class Plate
	{
		public:
			Plate(void);

			int run(int argc, char ** argv);
			void fatalSDLError(const char * context);
			void fatalError(const char * context,const char * err);

			~Plate(void);
			
		private:
			Display * d;
			bool isRunning;
	};
};
#endif //__PLATE_H__
