#include "core/object.h"

using namespace std;

void Object::Scale(double x, double y, double z) {
  T = ScaleMatrix(x,y,z) * T; 
  // Hacky approximation for surface area scaling
  surfaceArea *= x * y * z;
}

// Uniform scale
void Object::Scale(double s) {
  T = ScaleMatrix(s,s,s) * T; 
  // Hacky approximation for surface area scaling
  surfaceArea *= s * s * s;
}

void Object::Translate(double x, double y, double z) {
  T = TranslateMatrix(x,y,z) * T; 
}

void Object::RotateX(double a) {
  T = RotateXMatrix(a) * T; 
}

void Object::RotateY(double a) {
  T = RotateYMatrix(a) * T; 
}

void Object::RotateZ(double a) {
  T = RotateZMatrix(a) * T; 
}

void Object::addNormalMap(Texture *tx) {
  normalMap = tx;
};

void Object::addTextureMap(Texture *tx) {
  bsdf->m_tx = tx;
};

Vec Object::normalMapped(const Vec& canon_n, HitRec& rec) {
  if (normalMap == NULL) return canon_n;

  Vec tex_normal = 2 * normalMap->get(rec) - 1;
  Vec n = norm(alignTo(tex_normal, canon_n));
  return n;
}

Ray Object::rayTransform(const Ray& r) {
  return Ray(T_inv * r.p, T_inv % r.d, r.tMax);
}

Vec Object::sample(double *pdf, RNG& rng) {
  *pdf = 1 / surfaceArea;
  return T * Vec();
}

Vec Object::normalTransform(const Vec& n) {
  return norm(transpose(T_inv) * n);
}

void Object::finalize() {
  T_inv = invert(T);
  bounds = transformed(T, bounds);
}