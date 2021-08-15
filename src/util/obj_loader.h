#pragma once

#include <objects/triangle_mesh.h>
#include <vector>

namespace WavefrontOBJ {

void load(TriangleMesh &mesh, const char *fname);

}