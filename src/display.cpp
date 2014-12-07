#include "display.h"
#include "plate.h"
#include "tileset.h"
#include "tilelayer.h"
#include "texture.h"
#include "oglconsole.h"
#include <random>
using namespace PLATE;

const int tileForState[]={30*32+8,31*32+11,31*32+12,31*32+1,30*32+14};
static int px=0,py=0;
enum tileState
{
	TILE_STATE_ALIVE,
	TILE_STATE_DEADLY,
	TILE_STATE_SAFE,
	TILE_STATE_EMPTY,
	TILE_STATE_YOU
};

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
	tl=new TileLayer((Tileset *)ttset,64,32);
	tl2=new TileLayer((Tileset *)ttset,100,100,Vec2(-2,-2));

	ctset->setTile(0,1.0,0.0,0.0,0.5);
	ctset->setTile(1,0.0,0.0,1.0,0.5);
	ctset->setTile(2,1.0,0.0,1.0,0.5);
	ctset->setTile(3,0.0,1.0,0.0,0.5);	
	ctset->setTile(4,0.0,0.0,0.0,0.0);

	for (x=0;x<100;x++)
	{
		tl2->setTile(x,0,0);
		tl2->setTile(x,1,1);
		tl2->setTile(x,2,2);
		tl2->setTile(x,3,3);
		tl2->setTile(x,4,4);
	}
	std::default_random_engine generator;
	std::uniform_int_distribution<int> tiledist(0,1);
	std::uniform_int_distribution<int> tiledist2(0,32*3);
	for (y=5;y<100;y++)
		for (x=0;x<100;x++)
		{
			int tile=tiledist2(generator);
//			tile=tiledist(generator);
			tl2->setTile(x,y,tile);
		}
	

	for (x=0;x<64;x++)
	{
		for (y=0;y<32;y++)
		{
			tl->setTile(x,y,tileForState[tiledist(generator)*TILE_STATE_EMPTY]);
		}
	}

	SDL_SetWindowTitle(win,t);

	staticGLInitialization();

	/* Initialize viewPort size, sent to shader uniform of same name. */
	viewportSize[0] = 512; // XXX don't hard-code
	viewportSize[1] = 512; // XXX don't hard-code

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
	static int liframe=0;
	static Uint32 frametime;
	float fps;
	if (frames==0) frametime=SDL_GetTicks();
	//scroll=scroll+speed;
	SDL_GL_MakeCurrent(win,glctx);

        /* Clear the framebuffer */
	glClearColor(0.0f,0.0f,0.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	shaderProgram->use();
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glEnableVertexAttribArray(spaPosition);
	glEnableVertexAttribArray(spaTexture);
	glVertexAttribPointer(spaPosition,2,GL_FLOAT,false,16,(void *)0);
	glVertexAttribPointer(spaTexture,2,GL_FLOAT,false,16,(void *)8);
	glUniform2fv(spuViewportSize,1,viewportSize);
	//tl2->makeCurrent(this,scroll);
	//glDrawArrays(GL_TRIANGLES,0,6);
	tl->makeCurrent(this,scroll);
	glDrawArrays(GL_TRIANGLES,0,6);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,0);
	glUseProgram(0);


	//XXX LIFE CODE.  THIS CODE SHOULD NOT BE.
	int x,y;
	int ofs[]={	1,0,
			1,1,
			0,1,
			31,0,
			0,31,
			31,31,
			1,31,
			31,1};
	if (frames==50)
	{
		liframe++;
		for (x=0;x<32;x++)
		{
			for (y=0;y<32;y++)
			{
				int count=0;
				int i;
				for (i=0;i<8;i++)
				{
					if(tl->getTile((x+ofs[i*2])%32+32,(y+ofs[i*2+1])%32)==tileForState[TILE_STATE_ALIVE]) count++;
					if(px==((x+ofs[i*2])%32) && py==((y+ofs[i*2+1])%32)) count++;
				}
				if (tl->getTile(x+32,y)==tileForState[TILE_STATE_EMPTY])
				{
					if (count==0)
						tl->setTile(x,y,tileForState[TILE_STATE_EMPTY]);
					else if (count<2 || count>3)
						tl->setTile(x,y,tileForState[TILE_STATE_EMPTY]);
					else
						tl->setTile(x,y,tileForState[TILE_STATE_SAFE]);	
				}
				else
					tl->setTile(x,y,tileForState[TILE_STATE_ALIVE]);
			}
		}
		if (tl->getTile((32+px+int(speed.x))%32,(32+py+int(speed.y))%32)!=tileForState[TILE_STATE_ALIVE])
		{
			px=(32+px+int(speed.x))%32;
			py=(32+py+int(speed.y))%32;
		}
	}
	if (liframe>=100)
	{
		liframe=0;
		tl->setTile(px,py,tileForState[TILE_STATE_ALIVE]);
		for (x=0;x<32;x++)
		{
			for (y=0;y<32;y++)
			{
				int count=0;
				int i;
				for (i=0;i<8;i++)
				{
					if(tl->getTile((x+ofs[i*2])%32,(y+ofs[i*2+1])%32)==tileForState[TILE_STATE_ALIVE]) count++;
					if(px==((x+ofs[i*2])%32) && py==((y+ofs[i*2+1])%32)) count++;
				}
				if (tl->getTile(x,y)==tileForState[0])
				{
					if (count<2 || count>3)
						tl->setTile(x+32,y,tileForState[TILE_STATE_EMPTY]);
					else
						tl->setTile(x+32,y,tileForState[TILE_STATE_ALIVE]);
				}
				else
				{
					if (count==3)
						tl->setTile(x+32,y,tileForState[TILE_STATE_ALIVE]);
					else
						tl->setTile(x+32,y,tileForState[TILE_STATE_EMPTY]);
				}
			}
		}
		for (x=0;x<32;x++)
		{
			for (y=0;y<32;y++)
			{
				int count=0;
				int i;
				if (x==px && y==py && tl->getTile(x+32,y)==tileForState[TILE_STATE_EMPTY])
					std::cout<<"DAMAGE!"<<std::endl;
				for (i=0;i<8;i++)
				{
					if(tl->getTile((x+ofs[i*2])%32+32,(y+ofs[i*2+1])%32)==tileForState[TILE_STATE_ALIVE]) count++;
					if(px==((x+ofs[i*2])%32) && py==((y+ofs[i*2+1])%32)) count++;
				}
				if (tl->getTile(x+32,y)==tileForState[TILE_STATE_EMPTY])
				{
					if (count==0)
						tl->setTile(x,y,tileForState[TILE_STATE_EMPTY]);
					else if (count<2 || count>3)
						tl->setTile(x,y,tileForState[TILE_STATE_EMPTY]);
					else
						tl->setTile(x,y,tileForState[TILE_STATE_SAFE]);	
				}
				else
					tl->setTile(x,y,tileForState[TILE_STATE_ALIVE]);
			}
		}
	}
	tl->setTile(px,py,tileForState[TILE_STATE_YOU]);
	tl->refreshTiles();
	//tl2->render(this,scroll);
	//tl->render(this,scroll);
	OGLCONSOLE_Draw();
	SDL_GL_SwapWindow(win);
	frames++;
	if (frames==100) {
		frames=0;
		fps=100.0f/((float)(SDL_GetTicks()-frametime)/1000.0f);
//		std::cout<<"FPS:"<<fps<<std::endl;
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
		switch(k.keysym.scancode)
		{
			case SDL_SCANCODE_LEFT:
			case SDL_SCANCODE_RIGHT:
			case SDL_SCANCODE_A:
			case SDL_SCANCODE_D:
				speed.x=0;
				break;
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_DOWN:
			case SDL_SCANCODE_W:
			case SDL_SCANCODE_S:
				speed.y=0;
				break;
			default:
				break;
		}
	}
	else
	{
		switch(k.keysym.scancode)
		{
			case SDL_SCANCODE_ESCAPE:
				{
					SDL_Event e;
					e.type=SDL_QUIT;
					SDL_PushEvent(&e);
				}
				break;
			case SDL_SCANCODE_LEFT:
			case SDL_SCANCODE_A:
				speed.x=-1;
				break;
			case SDL_SCANCODE_RIGHT:
			case SDL_SCANCODE_D:
				speed.x=1;
				break;
			case SDL_SCANCODE_UP:
			case SDL_SCANCODE_W:
				speed.y=-1;
				break;
			case SDL_SCANCODE_DOWN:
			case SDL_SCANCODE_S:
				speed.y=1;
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
