#ifndef __TILELAYER_H__
#define __TILELAYER_H__
#include "platecommon.h"
#include "vec2.h"
namespace PLATE
{
	class TileLayer
	{
		public:
			TileLayer(Tileset * tiles, int width, int height, Vec2 parallax=Vec2(1,1), Vec2 scale=Vec2(1,1));

			void render(Display * d, Vec2 scroll);

			void setTile(int x, int y, int index);
			int getTile(int x, int y);
			void loadTiles(const int * src, int w, int h, int sx=0, int sy=0, int dx=0, int dy=0, int rowstride=0);
                        /* A way to batch-update the entire tile-layer to the GL */
                        void refreshTiles();

			void makeCurrent(Display * d, Vec2 scroll);

			~TileLayer();

		private:
			float inverseTileTextureSize[2];
			Tileset * t;
			int w;
			int h;
			int * tmap;
			unsigned char * ttex;
			GLuint ttexID;
			Vec2 p;
			Vec2 s;
	};
};
#endif //__TILELAYER_H__
