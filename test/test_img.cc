#include <iostream>
#include "util/image.h"

using namespace std;

int main() {
    Image im = Image(100, 100);
    Image im2 = Image(100, 100);
    for (int i = 0; i < 100; i++) {
        for (int j = 0; j < 100; j++) {
            Colour c = Colour(i/100.0, j/100.0, .25);
            im.set(i, j, c);
            im.hdrset(i, j, c);
        }
    }
    im.save("test.ppm");
    im.hdrsave("test2.ppm");
}