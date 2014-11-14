#include "tileset.h"
#include <iostream>
#include "display.h"
using namespace PLATE;
ColorTileset::ColorTileset(int tw, int th)
{
	this->tw=tw;
	this->th=th;
}
void ColorTileset::setTile(int index, SDL_Color c)
{
	tiles[index]=c;
}
void ColorTileset::setTile(int index, int r, int g, int b, int a)
{
	SDL_Color c;
	c.r=r;
	c.g=g;
	c.b=b;
	c.a=a;
	this->setTile(index,c);
}
void ColorTileset::setTile(int index, float r, float g, float b, float a)
{
	this->setTile(index,int(r*255),int(g*255),int(b*255),int(a*255));
}
void ColorTileset::renderTile(Display * d, int index, int x, int y, Vec2 scroll, Vec2 parallax, Vec2 scale)
{
	Vec2 tpos=(Vec2(x*tw,y*th)*scale)-(scroll*parallax);
	Vec2 text=(Vec2((x+1)*tw,(y+1)*th)*scale)-(scroll*parallax);
	SDL_Color c=tiles[index];
//	std::cout<<"Drawing quad ("<<tpos.x<<","<<tpos.y<<")-("<<text.x<<","<<text.y<<") in 4bRGBA("<<(int)c.r<<","<<(int)c.g<<","<<(int)c.b<<","<<(int)c.a<<")."<<std::endl;

	d->GLok("ColorTilest::renderTile{pre-glColor4b}");
	glColor4b(c.r,c.g,c.b,c.a);
	d->GLok("ColorTilest::renderTile{post-glColor4b}");
	glBegin(GL_QUADS);
	glVertex3f(tpos.x,tpos.y,0);
	glVertex3f(tpos.x,text.y,0);
	glVertex3f(text.x,text.y,0);
	glVertex3f(text.x,tpos.y,0);
	glEnd();
	d->GLok("ColorTilest::renderTile{post-glEnd}");
}
