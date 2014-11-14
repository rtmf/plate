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
void TileLayer::render(Display * d, Vec2 scroll)
{
	int x,y;
	for (y=0;y<h;y++)
		for (x=0;x<w;x++)
			t->renderTile(d,getTile(x,y),x,y,scroll,p,s);
}
TileLayer::~TileLayer()
{
	delete tmap;
}

