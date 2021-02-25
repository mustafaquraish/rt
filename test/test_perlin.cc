#include "../src/core/perlin.h"
#include "../src/util/image.h"


int main() {
  Image im = Image(512, 512);
  double scale = 4 / 511.0;


  double xPeriod = 5.0; //defines repetition of marble lines in x direction
  double yPeriod = 10.0; //defines repetition of marble lines in y direction
  //turbPower = 0 ==> it becomes a normal sine pattern
  double turbPower = 5.0; //makes twists
  double turbSize = 100.0; //initial size of the turbulence

  for (int i = 0; i < 512; i++) {
    for (int j = 0; j < 512; j++) {


      Vec p = Vec(i, j, 1) * scale;
      double pv = Perlin::layered(p, 7);
      double vv = map(pv, -1, 1, 0, 1);
      // pv = Perlin::turbulence(p);
      // Colour col = acos(1+pv);
      // Colour col = map(pv, -1, 1, 0, 1);

      double xyValue = i * xPeriod / 512.0 + j * yPeriod / 512.0 + turbPower * vv / 256.0;
      double sineValue = 256 * fabs(sin(xyValue * 3.14159));


      double   v= fabs(pv);
      Colour col = 0.5*(1 + sin(10*p.z + 10*pv));
      // C
      im.set(i, j, col);
      // im.set(i, j, v);
      // im.set(i, j, sineValue);
    }
  }
  im.save("perlin.ppm", false);
}