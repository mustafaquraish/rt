#pragma once

#include <cmath>

#include <core/vec.h>

using Colour = Vec3;

/**
 * Convert an RGB colour into a grayscale value based on luminance
 */
inline float luminance(const Colour& c) {
  return 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
}

/**
 * Convert Hue into an RGB Colour (assume saturation = value = 1)
 */
inline Colour from_hue(float hue) {
  hue *= 6.0;
  float C = 1.0;
  float X = C * (1.0 - abs(fmod(hue, 2.0) - 1.0));
  switch ((int) hue) {
    case 0: return Colour(1, X, 0);
    case 1: return Colour(X, C, 0);
    case 2: return Colour(0, C, X);
    case 3: return Colour(0, X, C);
    case 4: return Colour(X, 0, C);
    default: return Colour(C, 0, X);
  }
}