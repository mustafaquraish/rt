#pragma once

#include <util/image.h>

namespace PNG {

bool load(Image &, const char *);
bool save(Image &, const char *);

} 