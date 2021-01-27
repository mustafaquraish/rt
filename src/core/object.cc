#include "core/object.h"

using namespace std;

void Object::Scale(double x, double y, double z) {
  T = ScaleMatrix(x,y,z) * T; 
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

Ray Object::rayTransform(const Ray& r) {
  return Ray(T_inv * r.p, T_inv % r.d, r.tMax);
}

Vec Object::normalTransform(const Vec& n) {
  return norm(transpose(T_inv) * n);
}

void Object::finalize() {
  T_inv = invert(T);
  bounds = transformed(T, bounds);
}