#ifndef __MAP_CONVERTER_H__
#define __MAP_CONVERTER_H__

#include "util/image.h"
#include "core/math.h"

Image convertHeightMapToNormalMap(Image& heightIm, double strength=1);

#endif // __MAP_CONVERTER_H__
