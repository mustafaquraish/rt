#ifndef __RT_COMMON_H__
#define __RT_COMMON_H__

#include <iostream>

using std::cout;
using std::endl;

#define AGGREGATE BVH

#include "core/ray.h"
#include "core/object.h"
#include "core/camera.h"

#include "util/image.h"
#include "util/obj_loader.h"

// Individual Objects
#include "objects/plane.h"
#include "objects/sphere.h"
#include "objects/cube.h"
#include "objects/implicit.h"
#include "objects/triangle_mesh.h"

// Integrators
#include "integrators/debug_shader.h"


// Aggregates
#include "aggregates/primitive_list.h"
#include "aggregates/bvh.h"

#include "scene.h"

// Scene definitions
Scene *A2Scene(int sx, int sy);


#endif // __RT_COMMON_H__
