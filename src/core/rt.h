#pragma once

#include <iostream>

using std::cout;
using std::endl;

#include <core/camera.h>
#include <core/envmap.h>

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
#include <objects/medium.h>
#include <objects/constant_medium.h>
#include <objects/triangle_mesh.h>
#include <objects/bevel_curve.h>
#include <objects/surface_revolution.h>
#include <objects/displaced.h>
#include <objects/sdf.h>
#include <objects/csg.h>

// Renderers
#include <renderers/debug_shader.h>
#include <renderers/ambient_occlusion.h>
#include <renderers/direct_lighting.h>
#include <renderers/path.h>
#include <renderers/base_colour.h>
#include <renderers/photon_mapping.h>
#include <renderers/sppm.h>


// Aggregates
#include <aggregates/primitive_list.h>
#include <aggregates/bvh.h>
#include <aggregates/kdtree.h>

// Scene (this needs to be at the end)
#include <core/scene.h>

// Materials
#include <materials/basic.h>
#include <materials/emitter.h>
#include <materials/glossy.h>
#include <materials/hybrid.h>
#include <materials/orennayar.h>


