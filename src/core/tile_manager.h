#ifndef __TILE_MANAGER_H__
#define __TILE_MANAGER_H__

#include "util/params.h"

struct Tile {
  int xBegin, xEnd, yBegin, yEnd;
};
typedef Tile Window;

#define MIN_TILES 64

struct TileManager {
  TileManager(RenderParams& params) {
    int sx = params.get<int>("width");
    int sy = params.get<int>("height");

    // Compute window bounds in pixels
    window.xBegin = floor(sx * params.get<double>("xBegin"));
    window.yBegin = floor(sy * params.get<double>("yBegin"));
    window.xEnd = ceil(sx * params.get<double>("xEnd"));
    window.yEnd = ceil(sy * params.get<double>("yEnd"));

    tileSize = params.get<int>("tileSize");

    // Estimate the amount of tiles needed for the image
    int numPixelsInWindow = (window.xEnd - window.xBegin) * 
                            (window.yEnd - window.yBegin);
    int numPixelsInTile = tileSize * tileSize;
    int maxTiles = ceil( numPixelsInWindow / (double) numPixelsInTile);

    // If this is lower than some threshold, then change the tile size
    if (maxTiles < MIN_TILES) {
      tileSize = ceil( sqrt(numPixelsInWindow / (double) MIN_TILES) );
    }

    xTiles = ceil((window.xEnd - window.xBegin) / (double)tileSize);
    yTiles = ceil((window.yEnd - window.yBegin) / (double)tileSize);
    numTiles = xTiles * yTiles;
  }

  Tile getTile(int i) {
    int tx = i % xTiles;
    int ty = i / xTiles;
    
    Tile tile;
    tile.xBegin =     window.xBegin +   tx   * tileSize;
    tile.xEnd   = min(window.xBegin + (tx+1) * tileSize, window.xEnd);
    tile.yBegin =     window.yBegin +   ty   * tileSize;
    tile.yEnd   = min(window.yBegin + (ty+1) * tileSize, window.yEnd);

    return tile;
  }

  Window window;
  int tileSize, xTiles, yTiles;
  int numTiles;
};

#undef MIN_TILES

#endif // __TILE_MANAGER_H__
