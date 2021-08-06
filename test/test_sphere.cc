#include <iostream>
#include <objects/sphere.h>

using namespace std;

int main() {
    Object *s = new Sphere(new Lambertian(Colour(1,0,0)));
    cout << s->T << endl;
    s->Scale(3,4,5);
    s->Translate(1,2,3);
    cout << s->T << endl;
}