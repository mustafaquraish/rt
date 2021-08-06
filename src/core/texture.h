#pragma once

#include <core/primitive.h>
#include <unordered_map>
#include <util/image.h>
#include <ext/SimplexNoise.h>

struct Texture {
  virtual ~Texture() {};
  virtual Colour get(HitRec& rec) = 0;
  virtual Colour get(float u, float v);
  void saveImage(int size_x, int size_y, const char *filename);
};

struct ContantTexture : Texture {
  ContantTexture(Colour col) : col(col) {}
  virtual Colour get(HitRec& rec) { return col; }
  Colour col; 
};

struct ImageTexture : Texture {
  ImageTexture(const char *filename) { 
    im = RTImageList::getImage(filename);
    if (im == NULL) {
      im = new Image(filename);
      RTImageList::registerImage(filename, im);
    } 
  }
  virtual Colour get(HitRec& rec) { return im->get(rec.u, 1-rec.v); }
  Image *im = NULL; 
};

struct CheckerTexture : Texture {
  CheckerTexture(float scale = 0.1) : scale(scale) {};
  
  virtual Colour get(HitRec& rec) {
    int fx = rec.u / scale;
    int fy = rec.v / scale;
    bool fac = (fx + fy) % 2;
    return Colour(fac);
  }

  float scale;
};


struct PerlinTexture : Texture {
  PerlinTexture(PerlinType type = Layered, float scale = 8, int octaves = 4, 
                float persistence = 0.5, float seed=0) 
      : scale(scale), octaves(octaves), persistence(persistence), type(type), 
        seed(seed) {};
  
  virtual Colour get(HitRec& rec) {
    float x = rec.u * scale;
    float y = rec.v * scale;

    float perlin;

    /* It's more effcient to use the perlin 2D texture if we don't need 3D */
    if (seed == 0) perlin = Simplex::layered(octaves, persistence, x, y);
    else           perlin = Simplex::layered(octaves, persistence, x, y, seed);
    
    return Simplex::convertTo(perlin, type);
  }

  PerlinType type;
  float scale;
  float persistence;
  float seed;
  int octaves;
};

// 4-Dimensional perlin noise texture, uses `seed` \in [0,1] to perfectly loop
struct Perlin4DTexture : PerlinTexture {
  using PerlinTexture::PerlinTexture;
  
  virtual Colour get(HitRec& rec) {
    float x = rec.u * scale;
    float y = rec.v * scale;

    float theta = lerp(seed, 0.0, TAU);
    float z = cos(theta);
    float w = sin(theta);
    float perlin = Simplex::layered(octaves, persistence, x, y, z, w);
 
    return Simplex::convertTo(perlin, type);
  }
};

// 4-Dimensional perlin noise texture, uses `seed` \in [0,1] to perfectly loop
struct SDFTexture : Texture {
  SDFTexture(Colour col, float scale=1) : col(col), scale(scale) {};
  
  virtual Colour get(HitRec& rec) {
    return col * (1-rec.stepsRatio*scale);
  }
  Colour col;
  float scale;
};


