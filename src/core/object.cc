#include <core/object.h>
#include <core/bsdf.h>

#include <stdio.h>

using namespace std;

Object::~Object() {
  delete bsdf;
}

void Object::Scale(float x, float y, float z) {
  T = ScaleMatrix(x,y,z) * T; 
  // Hacky approximation for surface area scaling
  surfaceArea *= x * y * z;
}

// Uniform scale
void Object::Scale(float s) {
  T = ScaleMatrix(s,s,s) * T; 
  // Hacky approximation for surface area scaling
  surfaceArea *= s * s * s;
}

void Object::Translate(float x, float y, float z) {
  T = TranslateMatrix(x,y,z) * T; 
}

void Object::RotateX(float a) {
  T = RotateXMatrix(a) * T; 
}

void Object::RotateY(float a) {
  T = RotateYMatrix(a) * T; 
}

void Object::RotateZ(float a) {
  T = RotateZMatrix(a) * T; 
}

void Object::addNormalMap(Texture *tx) {
  normalMap = tx;
};

void Object::addTextureMap(Texture *tx) {
  bsdf->m_tx = tx;
};

void Object::setMaterial(BSDF *_bsdf, bool override) {
  if (bsdf && !override) {
    printf("Object already has a material applied, override=false\n");
    return;
  }
  delete bsdf;
  bsdf = _bsdf;
};

Vec3 Object::normalMapped(const Vec3& canon_n, HitRec& rec) {
  if (normalMap == NULL) return canon_n;

  Vec3 tex_normal = 2 * normalMap->get(rec) - 1;
  Vec3 n = normalized(align_to(tex_normal, canon_n));
  return n;
}

Ray Object::rayTransform(const Ray& r) {
  return Ray(T_inv * r.p, T_inv % r.d, r.tMax);
}

Vec3 Object::sample(float *pdf, RNG& rng) {
  *pdf = 1 / surfaceArea;
  return T * Vec3();
}

Vec3 Object::normalTransform(const Vec3& n) {
  return normalized(transpose(T_inv) * n);
}

void Object::finalize() {
  T_inv = invert(T);
  bounds = transformed(T, bounds);
}