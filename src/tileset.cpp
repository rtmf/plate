#include "tileset.h"
#include <iostream>
#include "display.h"
#include "texture.h"
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
TextureTileset::TextureTileset(Texture * t, int tw, int th)
{
	this->tw=tw;
	this->th=th;
	this->tex=t;
	this->trs=tex->getWidth()/tw;
}
void TextureTileset::renderBegin(Display * d, Vec2 scroll, Vec2 parallax, Vec2 scale)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	tex->Bind(&tcw,&tch);
	tcw/=tw;
	tch/=th;
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(scroll.x*parallax.x,scroll.y*parallax.y,0);
	glScalef(tw*scale.x,th*scale.y,1);
	glColor4f(1.0,1.0,1.0,1.0);
	glBegin(GL_QUADS);
}
void TextureTileset::renderTile(Display * d, int index, int x, int y)
{
	int ttx,tty;
	float tx0,tx1,ty0,ty1;
	tty=index/trs;
	ttx=index%trs;
	tx0=ttx*tcw;
	tx1=(ttx+1)*tcw;
	ty0=tty*tch;
	ty1=(tty+1)*tch;
	glTexCoord2f(tx0,ty0);
	glVertex3f(x,y,0);
	glTexCoord2f(tx1,ty0);
	glVertex3f(x+1,y,0);
	glTexCoord2f(tx1,ty1);
	glVertex3f(x+1,y+1,0);
	glTexCoord2f(tx0,ty1);
	glVertex3f(x,y+1,0);
}
void TextureTileset::renderEnd(Display * d)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glEnd();
	tex->Unbind();
}

