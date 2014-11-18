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
}
/* TODO: remove */
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
}
/* tw * th is the number of pixels found in a tile */
TextureTileset::TextureTileset(Texture * t, int tw, int th)
{
	this->tw=tw;
	this->th=th;
	this->tex=t;
	txc=tex->getWidth()/tw;
	tyc=tex->getHeight()/th;
	tcw=1.0/txc;
	tch=1.0/tyc;
}
void TextureTileset::renderBegin(Display * d, Vec2 scroll, Vec2 parallax, Vec2 scale)
{
	glEnable(GL_TEXTURE_2D);
	glClientActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, tex->getTextureName());

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(scroll.x*parallax.x,scroll.y*parallax.y,0);
	glScalef(tw*scale.x,th*scale.y,1);

	glColor4f(1.0,1.0,1.0,1.0);
}
/* TODO: remove */
void TextureTileset::renderTile(Display * d, int index, int x, int y)
{
	int ttx,tty;
	float tx0,tx1,ty0,ty1;
	tty=index/txc;
	ttx=index%txc;
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
void TextureTileset::getTileTexCoords(int index, float * texCoords)
{
	int ttx,tty;
	float tx0,tx1,ty0,ty1;
	tty=index/txc;
	ttx=index%txc;
	texCoords[0]=ttx*tcw;
	texCoords[1]=(ttx+1)*tcw;
	texCoords[2]=tty*tch;
	texCoords[3]=(tty+1)*tch;
}
void TextureTileset::renderEnd(Display * d)
{
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();

        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_TEXTURE_2D);
}

