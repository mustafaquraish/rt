#include "../src/util/map_converter.h"
#include "../src/util/image.h"

int main() {
  Image heightIm = Image("../assets/tex/sand-bump.ppm");
  Image normalIm = convertHeightMapToNormalMap(heightIm, 2);
  normalIm.save("perlin.ppm", false);
}