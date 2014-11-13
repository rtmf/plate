#include "display.h"
using namespace PLATE;
void Display::Display(Plate * p, int w, int h, const char * t)
{
	SDL_RendererInfo rendererInfo;
	plate=p;
	SDL_CreateWindowAndRenderer(w,h,SDL_WINDOW_OPENGL,&win,&rrr);
	if (win==NULL || rrr==NULL)
	{
		p->fatalSDLError("PLATE::Display::Display");
	}
	SDL_GetRendererInfo(rndr, &rendererInfo);
	if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
			(rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
	{
		p->fatalError("Could not set up accelerated render context / GL context");
	}
	width=w;
	height=h;
	SDL_SetWindowTitle(win,t);
	resetGL();
	resizeGL();
	clearGL();
}

void Display::resetGL(void)
{
	glShadeModel(GL_SMOOTH);
	this->clearGL();

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
}
void Display::resizeGL(void)
{
	Glfloat ratio,w,h;

	h=height;
	w=width;
	if (height==0)
		h=1;

	ratio=w/h;
	glViewport(0,0,(GLsizei)width,(GLsizei)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	gluPerspective(45.0f, ratio, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	this->clearGL();
}
void Display::clearGL(void)
{
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
void Display::resize(int w, int h)
{
	//XXX this is not finished
	this->resizeGL();
}
void Display::setTitle(const char * t);
{
	SDL_SetWindowTitle(win,t);
}
void Display::render(void)
{
	this->clearGL();

	glLoadIdentity();
	glTranslate(-1.5f,0.0f,-6.0f);

	glBegin(GL_TRIANGLES);
	{
		glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
	}
	glEnd();

	glTranslatef(3.0f, 0.0f, 0.0f);

	glBegin(GL_QUADS);
	{
		glVertex3f(-1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, 1.0f, 0.0f);
		glVertex3f(1.0f, -1.0f, 0.0f);
		glVertex3f(-1.0f, -1.0f, 0.0f);
	}
	glEnd();

	SDL_GL_SwapWindow(win);
}
 void Display::~Display(void)
{
	if (win)
		SDL_DestroyWindow(win);
}
