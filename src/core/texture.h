#pragma once

#include <core/primitive.h>
#include <unordered_map>
#include <util/image.h>
#include <ext/simplex.h>

/**
 * The abstract Texture class all textures should subclass from. 
 */
struct Texture {
  virtual ~Texture() {};

  virtual Colour get(const Vec2 &coords) { return Colour(0, 1, 0); };
  virtual Colour get(float u, float v) { return get(Vec2(u, v)); }
  
  // This is useful to be able to create create complex textures 
  // based on the normals/distance/etc. at the intersection point.
  virtual Colour get(const HitRec& rec) { return get(rec.uv); }

  void save(int size_x, int size_y, const char *filename);
};


struct ConstantTexture : Texture {
  ConstantTexture(Colour col) : m_col(col) {}
  Colour get(const Vec2 &coords) override { return m_col; }
private:
  Colour m_col; 
};

struct ImageTexture : Texture {
  ImageTexture(const char *filename) { 
    m_img = RTImageList::getImage(filename);
    if (m_img == NULL) {
      m_img = new Image(filename);
      RTImageList::registerImage(filename, m_img);
    } 
  }
  virtual Colour get(const Vec2 &coords) override { 
    return m_img->get(coords); 
  }
private:
  Image *m_img = nullptr; 
};

struct TintedImageTexture : ImageTexture {
  TintedImageTexture(const char *filename, Colour col) 
    : ImageTexture(filename), m_col(col) {}
  virtual Colour get(const Vec2 &coords) override { 
    return ImageTexture::get(coords) * m_col; 
  }
  Colour m_col;
};


struct CheckerTexture : Texture {
  CheckerTexture(float scale = 0.1) : scale(scale) {};
  
  virtual Colour get(const Vec2 &coords) override {
    int fx = coords.u / scale;
    int fy = coords.v / scale;
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
  
  virtual Colour get(const Vec2 &coords) override {
    Vec2 pos = coords * scale;

    float perlin;
    /* It's more effcient to use the perlin 2D texture if we don't need 3D */
    if (seed == 0) perlin = Simplex::layered(octaves, persistence, pos.u, pos.v);
    else           perlin = Simplex::layered(octaves, persistence, pos.u, pos.v, seed);
    
    return Simplex::transform(perlin, type);
  }

  PerlinType type;
  float scale;
  float persistence;
  float seed;
  int octaves;
};

// 4-Dimensional perlin noise texture, uses `seed` in [0,1] to perfectly loop
struct Perlin4DTexture : PerlinTexture {
  using PerlinTexture::PerlinTexture;
  
  virtual Colour get(const Vec2 &coords) override {
    Vec2 pos = coords * scale;

    float theta = lerp(seed, 0.0, TAU);
    float z = cos(theta);
    float w = sin(theta);
    float perlin = Simplex::layered(octaves, persistence, pos.u, pos.v, z, w);
 
    return Simplex::transform(perlin, type);
  }
};

// 4-Dimensional perlin noise texture, uses `seed` \in [0,1] to perfectly loop
struct SDFTexture : Texture {
  SDFTexture(Colour col, float scale=1) : col(col), scale(scale) {};
  
  virtual Colour get(const HitRec& rec) override {
    return col * (1-rec.stepsRatio*scale);
  }
  Colour col;
  float scale;
};


