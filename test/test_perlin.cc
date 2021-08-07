#include <../src/ext/simplex.h>
#include <../src/util/image.h>


int main() {
  Image im = Image(300, 300);
  float scale = 4 / 300.0;
  char buf[512];

  for (int s = 0; s < 1; s++) {
    
    float Z = inverseLerp((float)s, 0, 60)*3;
    float UZ = 3 - Z;

    for (int i = 0; i < 300; i++) {
      for (int j = 0; j < 300; j++) {

        Vec3 v1 = Vec3(i*scale, j*scale,  Z);
        Vec3 v2 = Vec3(i*scale, j*scale, UZ);

        float p1 = Simplex::layered(7.0,0.5, v1.x,v1.y,0,0);
        float p2 = Simplex::layered(7.0,0.5, v2.x,v2.y,0,0);
        float pa = inverseLerp((p1+p2)/2.0, -1, 1);

        im.set(i, j, pa);
      }
    }
    im.save("perlin.ppm", false);
    sprintf(buf, "convert perlin.ppm frames/%02d.png", s);
    system(buf);
  }
}