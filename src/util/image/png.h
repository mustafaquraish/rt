#pragma once

struct Image;

namespace PNG {

bool load(Image &, const char *);
bool save(Image &, const char *);

} 