#ifndef __VEC2_H__
#define __VEC2_H__
#include "platecommon.h"
namespace PLATE
{
	class Vec2
	{
		public:
			Vec2(float x=0, float y=0);
			Vec2(const Vec2 &v);

			Vec2 operator * (const Vec2 &v) const;
			Vec2 operator * (float s) const;
			Vec2 operator + (const Vec2 &v) const;
			Vec2 operator - (const Vec2 &v) const;
			
			float cross(const Vec2 &v) const;
			float dot(const Vec2 &v) const;

			void rotate(float theta);

			float magnitude(void);
			void normalize(void);

			float x,y;
	};
};
#define VEC2_EPSILON (0.0001f)
#endif //__VEC2_H__
