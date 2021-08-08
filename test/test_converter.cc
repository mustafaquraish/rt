#include <../src/util/map_converter.h>
#include <../src/util/image.h>

int main() {
  Image heightIm = Image("../assets/tex/sand-bump.ppm");
  Image normalIm = height_to_normal_map(heightIm, 2);
  normalIm.save("perlin.ppm", false);
}