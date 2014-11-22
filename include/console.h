#ifndef __CONSOLE_H__
#define __CONSOLE_H__
#include "platecommon.h"
#include "oglconsole.h"
namespace PLATE
{
	class Console
	{
		public:
			Console(Display * display);

			void render(void);
			void makeCurrent(void);
			void show(void);
			void hide(void);
			bool visible(void);
			void printf(const char * format, ...);
			void addHistory(const char * cmd);
			void yankHistory(void);
			void setInputLine(const char * input);
			void setCallback(void(*cbfun)(Display * d, Console * c, char * cmd));
			void callback(OGLCONSOLE_Console console, char * cmd);

			~Console(void);

		private:
			Display * d;
			OGLCONSOLE_Console c;
			void(*cbfunc)(Display * d, Console * c, char * cmd);
	};
};
#endif //__CONSOLE_H__
