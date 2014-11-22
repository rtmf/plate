#include "display.h"
#include "plate.h"
#include "tileset.h"
#include "tilelayer.h"
#include "texture.h"
#include "oglconsole.h"
#include <random>
using namespace PLATE;
/*
	These shaders are taken from
	http://media.tojicode.com/webgl-samples/js/webgl-tilemap.js
*/
const char * vertShader="\
precision mediump float;\
\
attribute vec2 position;\
attribute vec2 texture;\
\
varying vec2 pixelCoord;\
varying vec2 texCoord;\
\
uniform vec2 viewOffset;\
uniform vec2 viewportSize;\
uniform vec2 inverseTileTextureSize;\
uniform float inverseTileSize;\
\
void main(void)\
{\
	pixelCoord=(texture*viewportSize)+viewOffset;\
	texCoord=pixelCoord*inverseTileTextureSize*inverseTileSize;\
	gl_Position=vec4(position,0.0,1.0);\
}\
";
const char * fragShader="\
precision mediump float;\
\
varying vec2 pixelCoord;\
varying vec2 texCoord;\
\
uniform sampler2D tiles;\
uniform sampler2D sprites;\
\
uniform vec2 inverseTileTextureSize;\
uniform vec2 inverseSpriteTextureSize;\
uniform float tileSize;\
\
void main(void)\
{\
	vec4 tile = texture2D(tiles,texCoord);\
	if(tile.x==1.0&&tile.y==1.0) {discard;};\
	vec2 spriteOffset=floor(tile.xy*256.0)*tileSize;\
	vec2 spriteCoord=mod(pixelCoord,tileSize);\
	gl_FragColor=texture2D(sprites, (spriteOffset + spriteCoord) * inverseSpriteTextureSize);\
}\
";

Display::Display(Plate * p, int w, int h, const char * t)
{
	int x,y;
	SDL_RendererInfo rendererInfo;
	plate=p;
	SDL_CreateWindowAndRenderer(w,h,SDL_WINDOW_OPENGL,&win,&rrr);
	if (win==NULL || rrr==NULL)
	{
		p->fatalSDLError("PLATE::Display::Display");
	}
	SDL_GetRendererInfo(rrr, &rendererInfo);
	if ((rendererInfo.flags & SDL_RENDERER_ACCELERATED) == 0 ||
			(rendererInfo.flags & SDL_RENDERER_TARGETTEXTURE) == 0)
	{
		p->fatalError("PLATE::Display::Display","Could not set up accelerated render context / GL context");
	}
	glctx=SDL_GL_CreateContext(win);
        if (!gladLoadGLLoader(SDL_GL_GetProcAddress))
        {
          printf("could not load OpenGL 2.1 and needed extensions\n");
          exit(-1);
        }

	width=w;
	height=h;
	scroll=Vec2(0,0);
	speed=Vec2(0,0);
	
	SDL_GL_SetSwapInterval(1);

	tex=new Texture(this,"marioishBG16x16.png");
	ctset=new ColorTileset(16,16);
	ttset=new TextureTileset(tex,16,16);
	tl=new TileLayer((Tileset *)ttset,100,100);
	tl2=new TileLayer((Tileset *)ttset,100,100,Vec2(-2,-2));

	ctset->setTile(0,1.0,0.0,0.0,0.5);
	ctset->setTile(1,0.0,0.0,1.0,0.5);
	ctset->setTile(2,1.0,0.0,1.0,0.5);
	ctset->setTile(3,0.0,1.0,0.0,0.5);	
	ctset->setTile(4,0.0,0.0,0.0,0.0);

	for (x=0;x<100;x++)
	{
		tl->setTile(x,0,0);
		tl2->setTile(x,0,0);
		tl->setTile(x,1,1);
		tl2->setTile(x,1,1);
		tl->setTile(x,2,2);
		tl2->setTile(x,2,2);
		tl->setTile(x,3,3);
		tl2->setTile(x,3,3);
		tl->setTile(x,4,4);
		tl2->setTile(x,4,4);
	}
	std::default_random_engine generator;
	std::uniform_int_distribution<int> tiledist(0,4);
	std::uniform_int_distribution<int> tiledist2(0,(512/16)*(512/16)-1);
	for (y=5;y<100;y++)
		for (x=0;x<100;x++)
		{
			int tile=tiledist2(generator);
			tl->setTile(x,y,tile);
//			tile=tiledist(generator);
			tl2->setTile(x,y,tile);
		}
	

	SDL_SetWindowTitle(win,t);
	resetGL();
        tl->refreshTiles();
        tl2->refreshTiles();
	OGLCONSOLE_Create();
	vs=glCreateShader(GL_VERTEX_SHADER);
	fs=glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vs,1,&vertShader,NULL);
	glShaderSource(fs,1,&fragShader,NULL);
	glCompileShader(vs);
	glCompileShader(fs);
	sp=glCreateProgram();
	glAttachShader(sp,vs);
	glAttachShader(sp,fs);
	glLinkProgram(sp);
}
Plate * Display::getPlate(void)
{
	return plate;
}
SDL_Window * Display::getWindow(void)
{
	return win;
}
SDL_Renderer * Display::getRenderer(void)
{
	return rrr;
}
void Display::resetGL(void)
{
	SDL_GL_MakeCurrent(win,glctx);
	orthoGL();
	clearGL();
}
void Display::orthoGL(void)
{
	SDL_GL_MakeCurrent(win,glctx);

	glViewport(0,0,width,height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glOrtho (0,width,height,0,-1,1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	clearGL();
}
void Display::clearGL(void)
{
	SDL_GL_MakeCurrent(win,glctx);
	glClearColor(0.0f,0.0f,0.0f,0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
void Display::render(void)
{
	float w,h;
	static int frames=0;
	static Uint32 frametime;
	float fps;
	if (frames==0) frametime=SDL_GetTicks();
	scroll=scroll+speed;
	SDL_GL_MakeCurrent(win,glctx);
	resetGL();
	tl2->render(this,scroll);
	tl->render(this,scroll);
	OGLCONSOLE_Draw();
	SDL_GL_SwapWindow(win);
	frames++;
	if (frames==100) {
		frames=0;
		fps=100.0f/((float)(SDL_GetTicks()-frametime)/1000.0f);
		std::cout<<"FPS:"<<fps<<std::endl;
	}
}
void Display::setSize(int w, int h)
{
	//XXX this is not finished
	height=h;
	width=w;
	SDL_SetWindowSize(win,w,h);
	resetGL();
}
void Display::setTitle(const char * t)
{
	SDL_SetWindowTitle(win,t);
}
void Display::handleKey(SDL_KeyboardEvent k)
{
	if (k.type==SDL_KEYUP)
	{
		if (k.keysym.scancode==SDL_SCANCODE_LEFT ||
			k.keysym.scancode==SDL_SCANCODE_RIGHT)
			speed.x=0;
		if (k.keysym.scancode==SDL_SCANCODE_UP ||
			k.keysym.scancode==SDL_SCANCODE_DOWN)
			speed.y=0;
	}
	else
	{
		switch(k.keysym.scancode)
		{
			case SDL_SCANCODE_LEFT:
				speed.x=1;
				break;
			case SDL_SCANCODE_RIGHT:
				speed.x=-1;
				break;
			case SDL_SCANCODE_UP:
				speed.y=1;
				break;
			case SDL_SCANCODE_DOWN:
				speed.y=-1;
				break;
			default:
				break;
		}
	}
}
bool Display::GLok(const char* context, bool term_on_err)
{
	GLenum err=glGetError();
	if (err!=GL_NO_ERROR)
	{
		if (term_on_err)
			plate->fatalError(context,"some error");//(const char *)gluErrorString(err));
		else
			return false;
	}
	return true;
}
Display::~Display(void)
{
	if (win)
	{
		SDL_GL_DeleteContext(glctx);
		SDL_DestroyWindow(win);
	}
	OGLCONSOLE_Quit();
}
