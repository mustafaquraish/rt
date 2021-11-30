#pragma once

#include <core/colour.h>

namespace MERL {

struct BRDF {
    double *data;
};

bool read(BRDF& brdf, const char *filename);
Colour lookup(BRDF& brdf, float theta_in, float fi_in, float theta_out, float fi_out);

};