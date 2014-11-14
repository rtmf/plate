#include "vec2.h"
#include <math.h>

using namespace PLATE;

Vec2::Vec2(const Vec2 &v)
{
	x=v.x;
	y=v.y;
}

Vec2::Vec2(float x,float y)
{
	this->x=x;
	this->y=y;
}

Vec2 Vec2::operator * (const Vec2 &v) const
{
	return Vec2(x*v.x,y*v.y);
}

Vec2 Vec2::operator * (float s) const
{
	return Vec2(x*s,y*s);
}

Vec2 Vec2::operator + (const Vec2 &v) const
{
	return Vec2(x+v.x,y+v.y);
}

Vec2 Vec2::operator - (const Vec2 & v) const
{
	return Vec2(x-v.x,y-v.y);
}

float Vec2::cross(const Vec2 &v) const
{
	return (x * v.y) - (y * v.x);
}

float Vec2::dot(const Vec2 & v) const
{
	return (x * v.x) + (y * v.y);
}

void Vec2::rotate(float theta)
{
	float xt,yt;
	xt=(x*cosf(theta))-(y*sinf(theta));
	yt=(y*cosf(theta))+(x*sinf(theta));
	x=xt;
	y=yt;
}

float Vec2::magnitude(void)
{
	return sqrtf(x*x+y*y);
}

void Vec2::normalize(void)
{
	float mag=sqrtf(x*x+y*y);
	if (mag>VEC2_EPSILON)
	{
		x=x/mag;
		y=y/mag;
	}
}
