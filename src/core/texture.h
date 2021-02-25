#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "core/primitive.h"
#include <unordered_map>
#include "util/image.h"

struct Texture {
  virtual Colour get(HitRec& rec) = 0;
};

struct ImageTexture : Texture {
  ImageTexture(const char *fname) { im = new Image(fname); }
  Colour get(HitRec& rec) { return im->get(rec.u, 1-rec.v); }
private:
  Image *im = NULL; 
};

struct CheckerTexture : Texture {
  CheckerTexture(double scale = 0.1) : scale(scale) {};
  
  Colour get(HitRec& rec) {
    int fx = rec.u / scale;
    int fy = rec.v / scale;
    bool fac = (fx + fy) % 2;
    return Colour(fac);
  }

  double scale;
};

#endif // __TEXTURE_H__
