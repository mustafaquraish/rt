#pragma once

#include <core/primitive.h>
#include <vector>

// Returns a vector of TRIANGLES casted as `Primitive *`
std::vector<Primitive *> wavefrontObjLoader(const char *fname);

