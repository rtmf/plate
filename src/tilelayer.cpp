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
	verts = new Vertex[w*h*4];
	glGenBuffers(1, &vbo);
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
		}
	}

	/* Send vertex data to GL */
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, w * h * sizeof(*verts) * 4, verts, GL_STATIC_DRAW);
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
	glDeleteBuffers(1, &vbo);
	delete tmap;
}

