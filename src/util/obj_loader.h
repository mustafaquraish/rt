#pragma once

#include <core/primitive.h>
#include <vector>

// Returns a vector of TRIANGLES casted as `Primitive *`
std::vector<Primitive *> wavefrontObjLoader(const char *fname);

// struct TriangleMesh;
// void load_obj(TriangleMesh &mesh, const char *fname);
