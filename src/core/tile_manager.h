#pragma once

#include <util/params.h>

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
    window.xBegin = floor(sx * params.get<float>("xBegin"));
    window.yBegin = floor(sy * params.get<float>("yBegin"));
    window.xEnd = ceil(sx * params.get<float>("xEnd"));
    window.yEnd = ceil(sy * params.get<float>("yEnd"));

    tileSize = params.get<int>("tileSize");

    // Estimate the amount of tiles needed for the image
    int numPixelsInWindow = (window.xEnd - window.xBegin) * 
                            (window.yEnd - window.yBegin);
    int numPixelsInTile = tileSize * tileSize;
    int maxTiles = ceil( numPixelsInWindow / (float) numPixelsInTile);

    // If this is lower than some threshold, then change the tile size
    if (maxTiles < MIN_TILES) {
      tileSize = ceil( sqrt(numPixelsInWindow / (float) MIN_TILES) );
    }

    xTiles = ceil((window.xEnd - window.xBegin) / (float)tileSize);
    yTiles = ceil((window.yEnd - window.yBegin) / (float)tileSize);
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

