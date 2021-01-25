#include <iostream>
#include "../src/core/ray.h"

using namespace std;

int main() {
    Ray ray = Ray(Vec(0,3,0), Vec(2, 0, 1));
    cout << ray.at(0) << endl;
    cout << ray.at(0.5) << endl;
    cout << ray.at(1) << endl;
    cout << ray.at(0) << endl;
}