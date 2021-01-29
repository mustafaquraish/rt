#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__

#include "core/primitive.h"
#include "objects/triangle.h"
#include <vector>

// Returns a vector of TRIANGLES casted as `Primitive *`
Aggregate *wavefrontObjLoader(const char *fname);
// std::vector<Primitive *> wavefrontObjLoader(const char *fname);

#endif // __OBJ_LOADER_H__
