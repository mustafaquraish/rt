#include <iostream>
#include <../src/core/math.h>

using namespace std;

int main() {
    Vec3 a = Vec3(1);
    Vec3 b = Vec3(2);
    Vec3 c = Vec3(3);
    Colour C = Colour(4);

    cout << a << endl;
    cout << b << endl;
    cout << c << endl;

    Colour xj = c + C;
    cout << xj << endl;

    Vec3 d = a + b + c;
    cout << d << endl;

    float x = length(mult(a,b));
    cout << x << endl;

    cout << ((1/0.0) < 0) << endl;
    cout << ((1/0.0)) << endl;

    Matrix m1 = Matrix();
    Matrix m2 = m1 * 2;
    Matrix m3 = invert(m2);

    cout << m1 << endl;
    cout << m2 << endl;
    cout << m3 << endl;

    m2 += 7;

    cout << m2 << endl;


    Vec3 l = m3 * c;
    cout << l << endl;

    Matrix custom = Matrix(
        1, 3, 4, 5,
        5, 6, 7, 8,
        9, 0, 5, 6,
        1, 2, 3, 4
    );
    cout << custom << endl;
}