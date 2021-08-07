#pragma once

#include <stdio.h>
#include <string.h>
#include <core/math.h>
#include <unordered_map>

struct Image {
  Image(int sx, int sy);
  Image(char const *fname);
  ~Image();
  
  void save(char const *fname, bool gammaCorrect=true, float exposure=1.0);
  
  void set(int i, int j, Colour col);
  void splat(int i, int j, Colour col);
  
  Colour get(int i, int j);
  Colour get(float u, float v);
  Colour get(const Vec2& uv) { return get(uv.u, 1 - uv.v); }
  
  Vec2 size() const { return Vec2(sx, sy); }

  int sx, sy;
private:
  float *data;
};

struct RTImageList {
  inline static std::unordered_map<std::string, Image *> imgFileMapping;

  static void registerImage(std::string filename, Image *img);
  static Image *getImage(std::string filename);

  static void cleanup();
};

