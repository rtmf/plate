#ifndef __PLATE_H__
#define __PLATE_H__
#include "platecommon.h"
#include "display.h"
#include "oglconsole.h"
namespace PLATE
{
	class Plate
	{
		public:
			void Plate(void);

			void main(int argc, char ** argv);

			void ~Plate(void);
			
		private:
			Display * d;
	}
}
#endif //__PLATE_H__
