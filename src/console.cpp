#include "console.h"
#include "display.h"
using namespace PLATE;
Console::Console(Display * display)
{
	c=OGLCONSOLE_Create();
	makeCurrent();
	cbfunc=NULL;
	OGLCONSOLE_EnterKey(&(this->callback));
}
Console::~Console(void)
{
	OGLCONSOLE_Destroy(c);
}
void Console::render(void)
{
	OGLCONSOLE_Render(c);
}
void Console::makeCurrent(void)
{
	OGLCONSOLE_EditConsole(c);
}
void Console::show(void)
{
	makeCurrent();
	OGLCONSOLE_SetVisibility(true);
}
void Console::hide(void)
{
	makeCurrent();
	OGLCONSOLE_SetVisibility(false);
}
bool Console::visible(void)
{
	makeCurrent();
	return OGLCONSOLE_GetVisibility();
}
void Console::printf(const char * format, ...)
{
	va_list args;
	char output[4096];

	va_start(args,format);
	vsnprintf(output,4096,format,args);
	va_end(args);
	OGLCONSOLE_Output(c,"%s",output);
}
void Console::addHistory(const char * cmd)
{
	OGLCONSOLE_AddHistory(c,cmd);
}
void Console::yankHistory(void)
{
	OGLCONSOLE_YankHistory(c);
}
void Console::setInputLine(const char * input)
{
	makeCurrent();
	OGLCONSOLE_SetInputLine(input);
}
void Console::callback(OGLCONSOLE_Console console, char *cmd)
{
	if (cbfunc)
	{
		cbfunc(this->d,this,char * cmd);
	}
}
void Console::setCallback(void(*cbfun)(Display * d, Console * c, char * cmd))
{
	cbfunc=cbfun;
}
