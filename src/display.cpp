#include "display.h"
#include "plate.h"
#include "tileset.h"
#include "tilelayer.h"
#include "texture.h"
#include "oglconsole.h"
#include <random>
using namespace PLATE;

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
	
	SDL_GL_SetSwapInterval(0);

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

	staticGLInitialization();

	/* Initialize viewPort size, sent to shader uniform of same name. */
	viewportSize[0] = 640; // XXX don't hard-code
	viewportSize[1] = 480; // XXX don't hard-code

	tl->refreshTiles();
	tl2->refreshTiles();
	OGLCONSOLE_Create();

    /* Load tojimap shaders */
    shaderProgram = new ShaderProgram("shaders/vert-tojimap.glsl", "shaders/frag-tojimap.glsl");
    /* Load shader program variable locations */
    getShaderVariableLocations();
}
void Display::getShaderVariableLocations()
{
	spaPosition                 = shaderProgram->getAttribLocation("position");
	spaTexture                  = shaderProgram->getAttribLocation("texture");
	spuViewportSize             = shaderProgram->getUniformLocation("viewportSize");
	spuInverseSpriteTextureSize = shaderProgram->getUniformLocation("inverseSpriteTextureSize");
	spuTileSize                 = shaderProgram->getUniformLocation("tileSize");
	spuInverseTileSize          = shaderProgram->getUniformLocation("inverseTileSize");
	spuSprites                  = shaderProgram->getUniformLocation("sprites");
	spuTiles                    = shaderProgram->getUniformLocation("tiles");
	spuViewOffset               = shaderProgram->getUniformLocation("viewOffset");
	spuInverseTileTextureSize   = shaderProgram->getUniformLocation("inverseTileTextureSize");
}
GLuint Display::vbo = 0;
void Display::staticGLInitialization()
{
	/* Create a vertex buffer with a single quad filling the entire viewable volume
	 * in Normalized Device Coordinates, also spanning all of texture space as well.
	 */
	float verts[24] = {
	//   x   y  u  v
		-1, -1, 0, 1,
		 1, -1, 1, 1,
		 1,  1, 1, 0,

		-1, -1, 0, 1,
		 1,  1, 1, 0,
		-1,  1, 0, 0
	};
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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
void Display::render(void)
{
	float w,h;
	static int frames=0;
	static Uint32 frametime;
	float fps;
	if (frames==0) frametime=SDL_GetTicks();
	scroll=scroll+speed;
	SDL_GL_MakeCurrent(win,glctx);

        /* Clear the framebuffer */
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	shaderProgram->use();
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glEnableVertexAttribArray(spaPosition);
	glEnableVertexAttribArray(spaTexture);
	glVertexAttribPointer(spaPosition,2,GL_FLOAT,false,16,(void *)0);
	glVertexAttribPointer(spaTexture,2,GL_FLOAT,false,16,(void *)8);
	glUniform2fv(spuViewportSize,1,viewportSize);
	tl2->makeCurrent(this,scroll);
	glDrawArrays(GL_TRIANGLES,0,6);
	tl->makeCurrent(this,scroll);
	glDrawArrays(GL_TRIANGLES,0,6);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,0);
	glUseProgram(0);


	//tl2->render(this,scroll);
	//tl->render(this,scroll);
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
				speed.x=-1;
				break;
			case SDL_SCANCODE_RIGHT:
				speed.x=1;
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
	glDeleteBuffers(1,&vbo);
	if (win)
	{
		SDL_GL_DeleteContext(glctx);
		SDL_DestroyWindow(win);
	}
	OGLCONSOLE_Quit();
}
