#pragma once

#include <objects/triangle_mesh.h>
#include <vector>

namespace WavefrontOBJ {

void set_use_mesh_lights(bool enable);
void set_mesh_lights_scale(float scale);

void load(TriangleMesh &mesh, const char *fname);

}