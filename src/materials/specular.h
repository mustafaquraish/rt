/**
 * This file does NOT contain any actual material definitions, but rather
 * some helpers that are useful for computing speecular reflection and
 * refraction.
 */
#pragma once

#include <core/vec.h>

struct RNG;

namespace Specular {

Vec3 reflect(const Vec3& d, const Vec3& n, RNG& rng, float refl_sig=0);

/**
 * Compute the refracted ray, with optional diffuse refraction.
 * 
 * NOTE: We need to pass an RNG instance here since there is a possible
 *       chance that the ray will be reflected instead.
 */
Vec3 refract(const Vec3& d, const Vec3& n, float ref_idx, RNG &rng, float refl_sig=0);

}