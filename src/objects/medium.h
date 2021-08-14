#pragma once

#include <core/object.h>
#include <ext/simplex.h>

struct DensityTexture {
  virtual float get(const Vec3&) = 0;
};

struct ConstantDensityTexture : DensityTexture {
  ConstantDensityTexture(float density) : m_density(density) {}
  virtual float get(const Vec3&) { return m_density; };
  float m_density;
};

struct PerlinDensityTexture : DensityTexture {
  PerlinDensityTexture(float intensity, float scale=2) 
    : m_intensity(intensity), m_scale(scale) {}
  virtual float get(const Vec3& p) { 
    Vec3 pt = p * m_scale;
    float perlin = Simplex::layered(8, 0.5, pt.x, pt.y, pt.z);
    return Simplex::transform(perlin, Layered) * m_intensity;
  };
  
  float m_intensity;
  float m_scale;
};


struct Medium : Object {
  Medium(DensityTexture *density, Object *obj) 
    : m_obj(obj), m_density(density) { };
  bool hit(Ray &r, HitRec &rec) override;

  void finalize() override {
    m_obj->finalize();
    this->bounds = m_obj->bounds;
    Object::finalize();
  };

private:
  DensityTexture *m_density;
  Object *m_obj;
};

