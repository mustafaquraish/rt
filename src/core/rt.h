#pragma once

#include <iostream>

using std::cout;
using std::endl;


// Core stuff
#include <core/ray.h>
#include <core/object.h>
#include <core/camera.h>
#include <core/definitions.h>

// Utilities
#include <util/image.h>
#include <util/obj_loader.h>
#include <util/params.h>

// Individual Objects
#include <objects/plane.h>
#include <objects/sphere.h>
#include <objects/cylinder.h>
#include <objects/cube.h>
#include <objects/disc.h>
#include <objects/implicit.h>
#include <objects/lsystem.h>
#include <objects/triangle_mesh.h>
#include <objects/bevel_curve.h>
#include <objects/surface_revolution.h>
#include <objects/displaced.h>
#include <objects/sdf.h>

// Integrators
#include <integrators/debug_shader.h>
#include <integrators/ambient_occlusion.h>
#include <integrators/direct_lighting.h>
#include <integrators/path.h>
#include <integrators/base_colour.h>
#include <integrators/photon_mapping.h>
#include <integrators/sppm.h>


// Aggregates
#include <aggregates/primitive_list.h>
#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>

// Scene (this needs to be at the end)
#include <core/scene.h>

// Materials
#include <materials/lambertian.h>
#include <materials/emitter.h>
#include <materials/mirror.h>
#include <materials/transmissive.h>
#include <materials/orennayar.h>


