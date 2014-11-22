#include "tilelayer.h"
#include "tileset.h"
using namespace PLATE;
TileLayer::TileLayer(Tileset * tiles, int width, int height, Vec2 parallax, Vec2 scale)
{
	t=tiles;
	w=width;
	h=height;
	p=parallax;
	s=scale;
	tmap=new int[w*h]();
	ttex=new unsigned char[w*h*4]; 
	verts = new Vertex[w*h*4];
	glGenBuffers(1, &vbo);
	glGenTextures(1,&ttexID);
}
void TileLayer::setTile(int x,int y, int index)
{
	if (x>=0 && x<w && y>=0 && y<h)
		tmap[y*w+x]=index;
}
int TileLayer::getTile(int x,int y)
{
	if (x>=0 && x<w && y>=0 && y<h)
		return tmap[y*w+x];
	return 0;
}
void TileLayer::loadTiles(const int * src, int w, int h, int sx, int sy, int dx, int dy, int rowstride)
{
	int x,y;
	if ((w+dx)<=this->w && (h+dy)<=this->h && dx<=this->w && dy<=this->h && dx>0 && dy>0 && src != NULL)
	{
		for (y=0;y<h;y++)
			for(x=0;x<w;x++)
				tmap[(y+dy)*this->w+(x+dx)]=src[(y+sy)*(rowstride?rowstride:w+sx)+(x+sx)];
	}
}
void TileLayer::refreshTiles()
{
	/* TODO implement ColorTileset as well! */
	TextureTileset * tt = dynamic_cast<TextureTileset*>(t);
	if (tt == NULL)
	{
		printf("no support for other Tilesets than TextureTileset for now..\n");
		return;
	}
	/* First we must construct our vertex data */
	float texCoords[4];
	Vertex *vert = &verts[0];
	for (int y=0; y<h; y++)
	{
		for (int x=0; x<w; x++)
		{
			tt->getTileTexCoords(getTile(x, y), &texCoords[0]);
			vert[0] = Vertex(x,   y,   texCoords[0], texCoords[2]);
			vert[1] = Vertex(x+1, y,   texCoords[1], texCoords[2]);
			vert[2] = Vertex(x+1, y+1, texCoords[1], texCoords[3]);
			vert[3] = Vertex(x,   y+1, texCoords[0], texCoords[3]);
			vert += 4;
			int tpx,tpy;
			tt->getTileCoords(getTile(x,y),&tpx,&tpy);
			ttex[(x+y*w)*4]=tpx;
			ttex[(x+y*w)*4+1]=tpy;
		}
	}

	/* Send vertex data to GL */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, w * h * sizeof(*verts) * 4, verts, GL_STATIC_DRAW);

	glBindTexture(GL_TEXTURE_2D,ttexID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,w,h,0,GL_RGBA,GL_UNSIGNED_BYTE,ttex);
	glBindTexture(GL_TEXTURE_2D,0);

}

void TileLayer::render(Display * d, Vec2 scroll)
{
	/* TODO implement ColorTileset as well! */
	TextureTileset * tt = dynamic_cast<TextureTileset*>(t);
	if (tt == NULL)
		return;
	tt->renderBegin(d,scroll,p,s);

	glBindBuffer(GL_ARRAY_BUFFER, vbo);

	glEnableClientState(GL_VERTEX_ARRAY);
	glVertexPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, x));

	glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	glTexCoordPointer(2, GL_FLOAT, sizeof(Vertex), (void*)offsetof(Vertex, s));

	glDrawArrays(GL_QUADS, 0, w * h * 4);

	tt->renderEnd(d);
}
TileLayer::~TileLayer()
{
	glDeleteTextures(1,&ttexID);
	glDeleteBuffers(1, &vbo);
	delete tmap;
	delete ttex;
}

