#ifndef __MATERIAL_H__
#define __MATERIAL_H__

#include "core/math.h"

struct Material {
  Material(Colour c) : col(c){};
  inline Colour get() { return col; }

  Colour col;
};

#endif // __MATERIAL_H__
