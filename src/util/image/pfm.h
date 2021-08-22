#pragma once

#include <util/image.h>

namespace PFM {

bool load(Image &, const char *);
bool save(Image &, const char *);

}