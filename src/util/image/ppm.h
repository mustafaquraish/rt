#pragma once

#include <util/image.h>

namespace PPM {

bool load(Image &, const char *);
bool save(Image &, const char *);

}