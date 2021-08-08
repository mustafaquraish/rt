/**
 * Only supports 24-bit, Uncompressed BMP-3 format images.
 * Only images with a width and height that are multiples of 4 are 
 * supported for now, no padding support is implemented.
 * 
 * Use the following command (imagemagick) to convert to the expected format:
 *    convert <input_image> BMP3:<output_image>
 */

#pragma once

#include <util/image.h>

namespace BMP {

bool load(Image &, const char *);
bool save(Image &, const char *);

}