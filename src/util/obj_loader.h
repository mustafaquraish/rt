#pragma once

#include <objects/triangle_mesh.h>
#include <vector>

namespace WavefrontOBJ {

template <MeshType Type>
void load(TriangleMesh<Type> &mesh, const char *fname);

}