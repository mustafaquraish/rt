#include "../src/ext/PerlinNoise.h"
#include "../src/util/image.h"

using namespace siv;

int main() {
  Image im = Image(512, 512);
  int pscale = 4;
  BasicPerlinNoise<double> perlin;
  for (int i = 0; i < 512; i++) {
    for (int j = 0; j < 512; j++) {
      Colour col = perlin.noise2D_0_1(i/16., j/16.);
      im.set(i, j, col);
    }
  }
  im.save("perlin.ppm", false);
}