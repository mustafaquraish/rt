#ifndef __OBJ_LOADER_H__
#define __OBJ_LOADER_H__

#include "core/primitive.h"
#include <vector>

// Returns a vector of TRIANGLES casted as `Primitive *`
std::vector<Primitive *> wavefrontObjLoader(const char *fname);

#endif // __OBJ_LOADER_H__
