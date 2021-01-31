#ifndef __RT_COMMON_H__
#define __RT_COMMON_H__

#include <iostream>

using std::cout;
using std::endl;

#define AGGREGATE BVH

// Core stuff
#include "core/ray.h"
#include "core/object.h"
#include "core/camera.h"

// Utilities
#include "util/image.h"
#include "util/obj_loader.h"

// Individual Objects
#include "objects/plane.h"
#include "objects/sphere.h"
#include "objects/cylinder.h"
#include "objects/cube.h"
#include "objects/disc.h"
#include "objects/implicit.h"
#include "objects/lsystem.h"
#include "objects/triangle_mesh.h"
#include "objects/bevel_curve.h"
#include "objects/surface_revolution.h"

// Integrators
#include "integrators/debug_shader.h"
#include "integrators/ambient_occlusion.h"


// Aggregates
#include "aggregates/primitive_list.h"
#include "aggregates/bvh.h"

// Scene (this needs to be at the end)
#include "scene.h"

// Scene definitions
Scene *A2Scene(int sx, int sy);
Scene *RoomScene(int sx, int sy);
Scene *OcclusionScene(int sx, int sy);
Scene *ImplicitScene(int sx, int sy);
Scene *SORScene(int sx, int sy);
Scene *LsystemsScene(int sx, int sy);


#endif // __RT_COMMON_H__
