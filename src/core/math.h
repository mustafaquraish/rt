/***
 * Main header file to include to get all the math stuff. This file includes
 * `math2d.h` and `math3d.h`, so you do not need those separately.
 * 
 *  Including this header file gives you access to:
 *    - Common utility functions: min(), max(), clamp01()
 *    - Global Constants: PI, TAU, TOL
 *    - 2D vectors (Vec2) and 2D Homogenous Matrices (Matrix3)
 *    - 3D vectors (Vec3) and 3D Homogenous Matrices (Matrix4)
 *        - Most vector operators overloaded already
 *        - Matrix vector products, including homogenous multiplication
 *        - Matrix inversions
 *    - Access to a `Colour` object (alias of Vec3)
 *    - Linear / Bilinear interpolation, transforming ranges
 */

#pragma once

#include <iostream>
#include <math.h>
#include <stdlib.h>

#include <core/matrix.h>
#include <core/colour.h>

/**************************** Global Constants *******************************/

#define PI     3.141592653589793
#define TAU    6.283185307179586
#define INV_PI 0.3183098861837907
#define TOL    (10e-4)
#define EPS    (10e-3)

/**************************** Common Functions *******************************/

/** 
 * Convert radians to degrees
 */
inline float degrees(const float radians) { 
  return radians * 180.0 / PI; 
}

/** 
 * Convert degrees to radians
 */
inline float radians(const float degrees) { 
  return degrees * PI / 180.0; 
}

inline float min(const float a, const float b) { return (a < b) ? a : b; }
inline float max(const float a, const float b) { return (a > b) ? a : b; }

/**
 * Restrict a value to be between 0 and 1
 */
inline float clamp01(const float a) { 
  return min(1, max(a, 0)); 
}

/**
 * Restrict the value to be in the given range
 */
inline float clamp(const float a, const float mn, const float mx) { 
  return min(mx, max(a, mn)); 
}

/***************************** Interpolation *********************************/

/**
 * Linearly interpolate the values `a` and `b` with a factor of `fac`
 */
template<typename T>
inline T lerp(float fac, const T a, const T b) {
  return a + fac * (b - a);
}

/**
 * Linearly map a value from the range [sMin-sMax] to [0-1]
 * This is the inverse of `lerp()`
 */
template<typename T>
inline T inverseLerp(const T s, float sMin=-1, float sMax=1) {
  return (s - sMin) / (sMax - sMin);
}

/**
 * Bilinear interpolation between 4 values
 */
template<typename T>
inline T bilerp(float facX, float facY, const T xy, const T Xy,
                                        const T xY, const T XY) {
  return lerp(facY, lerp(facX, xy, Xy), lerp(facX, xY, XY));
}

/**
 * Trilinear interpolation between 8 values
 */
template<typename T>
inline T trilerp(float facX, float facY, float facZ, 
                 const T xyz, const T Xyz, const T xYz, const T XYz,
                 const T xyZ, const T XyZ, const T xYZ, const T XYZ) {
  return lerp(facZ, bilerp(facX, facY, xyz, Xyz, xYz, XYz), 
                    bilerp(facX, facY, xyZ, XyZ, xYZ, XYZ));
}

/** 
 * Map value(s) from [sMin-sMax] to the new range [tMin-tMax]
 */
template<typename T>
inline T map(const T a, float sMin, float sMax, 
                        float tMin, float tMax) {
  float ratio = (tMax - tMin) / (sMax - sMin);
  return (a - sMin) * ratio + tMin; 
}

/***************************** Conversions ************************************/

inline Vec2 from_polar(float theta) {
  return { cos(theta), sin(theta) };
}

inline Vec3 from_polar(float theta, float phi) {
  return { cos(theta) * sin(phi), sin(theta) * sin(phi), cos(phi) };
}

/***************************** Quadratic Solving *****************************/

inline bool solve_quadratic(float a, float b, float c,
                            float *l1, float *l2) {
  float disc = b*b - 4*a*c;
  if (disc < 0) return false; // No solutions
  float root = sqrt(disc);
  *l1 = (-b - root) / (2 * a);
  *l2 = (-b + root) / (2 * a);
  return true;
}
