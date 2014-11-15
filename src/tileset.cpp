#include "tileset.h"
#include <iostream>
#include "display.h"
using namespace PLATE;
ColorTileset::ColorTileset(int tw, int th)
{
	this->tw=tw;
	this->th=th;
}
void ColorTileset::setTile(int index, float r, float g, float b, float a)
{
	color4f c={r,g,b,a};
	setTile(index,c);
}
void ColorTileset::setTile(int index,color4f c)
{
	tiles[index]=c;
}
void ColorTileset::renderBegin(Display * d, Vec2 scroll, Vec2 parallax, Vec2 scale)
{
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(scroll.x*parallax.x,scroll.y*parallax.y,0);
	glScalef(tw*scale.x,th*scale.y,1);
	glBegin(GL_QUADS);
}
void ColorTileset::renderTile(Display * d, int index, int x, int y)
{
	auto ci=tiles.find(index);
	color4f c={0,0,0,0};
	if (ci!=tiles.end()) c=ci->second;
//	std::cout<<"Drawing quad ("<<tpos.x<<","<<tpos.y<<")-("<<text.x<<","<<text.y<<") in 4bRGBA("<<(int)c.r<<","<<(int)c.g<<","<<(int)c.b<<","<<(int)c.a<<")."<<std::endl;

	glColor4f(c.r,c.g,c.b,c.a);
	glVertex3f(x,y,0);
	glVertex3f(x+1,y,0);
	glVertex3f(x+1,y+1,0);
	glVertex3f(x,y+1,0);
}
void ColorTileset::renderEnd(Display * d)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnd();
}