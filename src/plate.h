#ifndef __PLATE_H__
#define __PLATE_H__
#include "platecommon.h"
//#include "oglconsole.h"
//XXX not using this yet
#include <iostream>
#include <vector>
#include <string>
namespace PLATE
{
	class Plate
	{
		public:
			Plate(void);

			int run(int argc, char ** argv);
			void fatalSDLError(const char * context);
			void fatalError(const char * context,const char * err);
			void logError(const char * context,const char * err);
			Display * getDisplay(void);

			std::vector<std::string> getTextureSearchPath(void);
			std::string getPathSep(void);

			~Plate(void);
			
		private:
			std::vector<std::string> textureSearchPath;
			std::string pathsep;
			Display * d;
			bool isRunning;
	};
};
#endif //__PLATE_H__
