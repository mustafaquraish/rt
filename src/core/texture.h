#ifndef __TEXTURE_H__
#define __TEXTURE_H__

#include "core/primitive.h"
#include <unordered_map>
#include "util/image.h"
#include "core/perlin.h"

struct Texture {
  virtual Colour get(HitRec& rec) = 0;
  virtual Colour get(double u, double v);
  void saveImage(int size_x, int size_y, const char *filename);
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

enum PerlinType {Layered, Turbulence, Marble};
struct PerlinTexture : Texture {
  PerlinTexture(PerlinType type = Layered, double scale = 8, int octaves = 4, 
                double persistence = 0.5, double lacunarity = 2, double seed=0) 
      : scale(scale), octaves(octaves), persistence(persistence), 
        lacunarity(lacunarity), type(type), seed(seed) {};
  
  Colour get(HitRec& rec) {
    double x = rec.u * scale;
    double y = rec.v * scale;
    double z = seed * scale;
    double perlin = Perlin::layered(x, y, z, octaves, persistence, lacunarity); 
    switch (type) {
    case Layered:    return map(perlin, -1, 1, 0, 1); break;
    case Turbulence: return abs(perlin);              break;
    case Marble:     return (1+sin(10*perlin)) / 2;   break;    
    default:         return perlin; break;
    }
  }

  PerlinType type;
  double scale;
  double persistence;
  double lacunarity;
  double seed;
  int octaves;
};

#endif // __TEXTURE_H__
