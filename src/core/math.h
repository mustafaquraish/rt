/**
 * One-Header math library for ray-tracing. Includes:
 *    - Common utility functions: min(), max(), clamp01()
 *    - Global Constants: PI, TAU, TOL
 *    - Random number generation: rand01()
 *    - Vector math definitions: Vec, dot(), cross(), ...
 *    - Matrix math definitions: Matrix, mat-vec / mat-mat mult, 4x4 invert
 *    - Transformation matrices: Scale, Translate, Rotate X/Y/Z
 *    - Linear / Bilinear interpolation, transforming ranges
 *    - Quadratic Solving: solveQuadratic()
 */

#ifndef __RT_MATH_H__
#define __RT_MATH_H__

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <iomanip>      // std::setprecision


/**************************** GLOBAL CONSTANTS *******************************/

#define PI     3.141592653589793
#define INV_PI 0.3183098861837907
#define TAU    6.283185307179586
#define TOL    (10e-6)

/**************************** COMMON FUNCTIONS *******************************/

inline double degrees(const double radians) { return radians * 180.0 / PI; }
inline double radians(const double degrees) { return degrees * PI / 180.0; }

inline double min(const double a, const double b) { return (a < b) ? a : b; }
inline double max(const double a, const double b) { return (a > b) ? a : b; }
inline double clamp01(const double a) { return min(1, max(a, 0)); }
inline double clamp(const double a, const double _min, const double _max) { 
  return min(_max, max(a, _min)); 
}

/************************** RANDOM NUMBERS ***********************************/

inline double rand01(void) {
  static int x = 123456789;
  static int y = 362436069;
  static int z = 521288629;
  static int w = 88675123;
  int t;
  t = x ^ (x << 11);
  x = y;
  y = z;
  z = w;
  return (w = w ^ (w >> 19) ^ (t ^ (t >> 8))) / 2147483647.0;
}

/**************************** VECTOR *****************************************/

union Vec {
  // Access vector elements in array format
  double V[3];
  // Access X, Y, Z components
  struct {
    double x, y, z;
  };
  // Use a vector as a colour
  struct {
    double r, g, b;
  };

  Vec() : x(0), y(0), z(0){};
  Vec(double v) : x(v), y(v), z(v){};
  Vec(double x, double y, double z) : x(x), y(y), z(z){};

  double &operator[] (int idx) { return V[idx]; }
  double operator[] (int idx) const { return V[idx]; };
};

typedef Vec Colour;

inline std::ostream &operator<<(std::ostream &os, const Vec& t) { 
  os << t.x << " " << t.y << " " << t.z; 
  return os; 
}

inline Vec operator+(const Vec& v1, const Vec& v2) { return Vec(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
inline Vec operator+(const Vec& v, const double t) { return Vec(v.x + t, v.y + t, v.z + t); }
inline Vec operator+(const double t, const Vec& v) { return Vec(v.x + t, v.y + t, v.z + t); }

inline Vec operator-(const Vec& v1) { return Vec(-v1.x, -v1.y, -v1.z); }
inline Vec operator-(const Vec& v1, const Vec& v2) { return Vec(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
inline Vec operator-(const Vec& v, const double t) { return Vec(v.x - t, v.y - t, v.z - t); }
inline Vec operator-(const double t, const Vec& v) { return Vec(v.x - t, v.y - t, v.z - t); }

inline Vec operator*(const Vec& v1, const Vec& v2) { return Vec(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z); }
inline Vec operator*(const double t, const Vec& v) { return Vec(t * v.x, t * v.y, t * v.z); }
inline Vec operator*(const Vec& v, const double t) { return Vec(t * v.x, t * v.y, t * v.z); }

inline Vec operator/(const double t, const Vec& v) { return Vec(v.x / t, v.y / t, v.z / t); }
inline Vec operator/(const Vec& v, const double t) { return Vec(v.x / t, v.y / t, v.z / t); }

inline double length(const Vec& a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
inline double lengthSq(const Vec& a) { return a.x * a.x + a.y * a.y + a.z * a.z; }
inline Vec norm(const Vec& a) { return a * (1 / sqrt(a.x * a.x + a.y * a.y + a.z * a.z)); }
inline Vec mult(const Vec& a, const Vec& b) { return Vec(a.x * b.x, a.y * b.y, a.z * b.z); }

inline Vec floor(const Vec& a) { return Vec(floor(a.x), floor(a.y), floor(a.z)); }
inline Vec sqrt(const Vec& v) { return Vec(sqrt(v.x), sqrt(v.y), sqrt(v.z)); }
inline double dot(const Vec& a, const Vec& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
inline Vec cross(const Vec& a, const Vec& b) { 
  return Vec( a.y * b.z - a.z * b.y, 
             -a.x * b.z + a.z * b.x, 
              a.x * b.y - a.y * b.x); 
}

inline Vec& operator+=(Vec &a, const Vec& b) { return a = a + b; }
inline Vec& operator+=(Vec &a, const double& b) { return a = a + b; }

inline Vec& operator-=(Vec &a, const Vec& b) { return a = a - b; }
inline Vec& operator-=(Vec &a, const double b) { return a = a - b; }

inline Vec& operator*=(Vec &a, const Vec& b) { return a = a * b; }
inline Vec& operator*=(Vec &a, const double b) { return a = a * b; }

inline Vec& operator/=(Vec &a, const double b) { return a = a / b; }


inline Vec clamp01(const Vec& a) {
  return Vec(min(1, max(a.x, 0)),
             min(1, max(a.y, 0)),
             min(1, max(a.z, 0)));
}

inline Vec clamp01(const Vec& a, double _min, double _max) {
  return Vec(min(_max, max(a.x, _min)),
             min(_max, max(a.y, _min)),
             min(_max, max(a.z, _min)));
}

// Return the index of the maximum element
inline int maxIndex(const Vec& a) {
  if (a.x >= a.y && a.x >= a.z) return 0;
  if (a.y >= a.x && a.y >= a.z) return 1;
  return 2;
}

// Return the maximum component of the vector 
inline double max(const Vec& a) {
  return max(a.x, max(a.y, a.z));
}

// Component-wise maximum of 2 vectors
inline Vec cmpWiseMax(const Vec& a, const Vec& b) {
  return Vec(
    max(a.x, b.x),
    max(a.y, b.y),
    max(a.z, b.z)
  );
}

/**************************** MATRIX *****************************************/

struct Matrix {
  double M[4][4];

  // Identity matrix if initialized without params.
  Matrix() {
    for (int i = 0; i < 4; i++) 
      for (int j = 0; j < 4; j++)
        M[i][j] = (i == j);
  }

  // Initialize with constant value for all elements
  Matrix(double v) {
    for (int i = 0; i < 4; i++) 
      for (int j = 0; j < 4; j++)
        M[i][j] = v;
  }

  // Initialize with individual values for each element
  Matrix(double m00, double m01, double m02, double m03,
         double m10, double m11, double m12, double m13,
         double m20, double m21, double m22, double m23,
         double m30, double m31, double m32, double m33) {
    M[0][0] = m00, M[0][1] = m01, M[0][2] = m02, M[0][3] = m03;
    M[1][0] = m10, M[1][1] = m11, M[1][2] = m12, M[1][3] = m13;
    M[2][0] = m20, M[2][1] = m21, M[2][2] = m22, M[2][3] = m23;
    M[3][0] = m30, M[3][1] = m31, M[3][2] = m32, M[3][3] = m33; 
  }

  double* operator[](int idx) { return M[idx]; }
  double const* operator[](int idx) const { return M[idx]; };

};

inline Matrix operator+(const Matrix& a, const Matrix& m2) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
      m[i][j] = a[i][j] + m2[i][j];
  return m;
}
inline Matrix operator+(const Matrix& a, double d) { return a + Matrix(d); };
inline Matrix operator+(double d, const Matrix& a) { return a + Matrix(d); };


inline Matrix operator-(const Matrix& a, const Matrix& b) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
      m[i][j] = a[i][j] - b[i][j];
  return m;
}
inline Matrix operator-(const Matrix& a, double d) { return a - Matrix(d); };
inline Matrix operator-(double d, const Matrix& a) { return a - Matrix(d); };

inline Matrix operator*(const Matrix& a, double t) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      m[i][j] = a[i][j] * t;
    }
  return m;
}
inline Matrix operator*(double t, const Matrix& a) { return a * t; }

inline Matrix operator/(const Matrix& a, double t) { return a * (1 / t); }

inline Matrix& operator+=(Matrix &a, const Matrix b) { return a = a + b; }
inline Matrix& operator+=(Matrix &a, double b) { return a = a + b; }

inline Matrix& operator-=(Matrix &a, const Matrix b) { return a = a - b; }
inline Matrix& operator-=(Matrix &a, double b) { return a = a - b; }

inline Matrix& operator*=(Matrix &a, double b) { return a = a * b; }
inline Matrix& operator/=(Matrix &a, double b) { return a = a / b; }

inline Matrix transpose(const Matrix &a) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      m[i][j] = a[j][i];
    }
  return m;
}

/************* MATRIX-VEC / MATRIX-MATRIX MULTIPLICATION *********************/

inline Matrix operator*(const Matrix& a, const Matrix& b) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      m[i][j] = a[i][0] * b[0][j];
      m[i][j] += a[i][1] * b[1][j];
      m[i][j] += a[i][2] * b[2][j];
      m[i][j] += a[i][3] * b[3][j];
    }
  return m;
}

// NOTE: Assuming w component is always 0, no homogenous-normalizing here.
inline Vec operator*(const Matrix& m, const Vec& v) {
  return Vec(
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]
  );
}

// This transforms the Vector **WITHOUT** the translation component  
inline Vec operator%(const Matrix& m, const Vec& v) {
  return Vec(
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
  );
}

inline std::ostream& operator<<(std::ostream& os, const Matrix& m) {
  os << "[";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) 
      os << std::setprecision(4) << std::setfill('0') << m[i][j] << ", ";
    if (i != 3) 
      os << "\n ";
  }
  os << "]";
  return os;
}

inline void printMatrix(const Matrix& m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) 
      printf("%2.3e, ", m[i][j]);
    printf("\n");
  }
}

/*************************** MATRIX INVERSION ********************************/

// Source: https://stackoverflow.com/questions/1148309
inline Matrix invert(const Matrix &m) {
  Matrix inv = Matrix();

  inv[0][0] =  m[1][1] * m[2][2] * m[3][3] - m[1][1] * m[2][3] * m[3][2] - m[2][1] * m[1][2] * m[3][3] + m[2][1] * m[1][3] * m[3][2] + m[3][1] * m[1][2] * m[2][3] - m[3][1] * m[1][3] * m[2][2];
  inv[1][0] = -m[1][0] * m[2][2] * m[3][3] + m[1][0] * m[2][3] * m[3][2] + m[2][0] * m[1][2] * m[3][3] - m[2][0] * m[1][3] * m[3][2] - m[3][0] * m[1][2] * m[2][3] + m[3][0] * m[1][3] * m[2][2];
  inv[2][0] =  m[1][0] * m[2][1] * m[3][3] - m[1][0] * m[2][3] * m[3][1] - m[2][0] * m[1][1] * m[3][3] + m[2][0] * m[1][3] * m[3][1] + m[3][0] * m[1][1] * m[2][3] - m[3][0] * m[1][3] * m[2][1];
  inv[3][0] = -m[1][0] * m[2][1] * m[3][2] + m[1][0] * m[2][2] * m[3][1] + m[2][0] * m[1][1] * m[3][2] - m[2][0] * m[1][2] * m[3][1] - m[3][0] * m[1][1] * m[2][2] + m[3][0] * m[1][2] * m[2][1];
  inv[0][1] = -m[0][1] * m[2][2] * m[3][3] + m[0][1] * m[2][3] * m[3][2] + m[2][1] * m[0][2] * m[3][3] - m[2][1] * m[0][3] * m[3][2] - m[3][1] * m[0][2] * m[2][3] + m[3][1] * m[0][3] * m[2][2];
  inv[1][1] =  m[0][0] * m[2][2] * m[3][3] - m[0][0] * m[2][3] * m[3][2] - m[2][0] * m[0][2] * m[3][3] + m[2][0] * m[0][3] * m[3][2] + m[3][0] * m[0][2] * m[2][3] - m[3][0] * m[0][3] * m[2][2];
  inv[2][1] = -m[0][0] * m[2][1] * m[3][3] + m[0][0] * m[2][3] * m[3][1] + m[2][0] * m[0][1] * m[3][3] - m[2][0] * m[0][3] * m[3][1] - m[3][0] * m[0][1] * m[2][3] + m[3][0] * m[0][3] * m[2][1];
  inv[3][1] =  m[0][0] * m[2][1] * m[3][2] - m[0][0] * m[2][2] * m[3][1] - m[2][0] * m[0][1] * m[3][2] + m[2][0] * m[0][2] * m[3][1] + m[3][0] * m[0][1] * m[2][2] - m[3][0] * m[0][2] * m[2][1];
  inv[0][2] =  m[0][1] * m[1][2] * m[3][3] - m[0][1] * m[1][3] * m[3][2] - m[1][1] * m[0][2] * m[3][3] + m[1][1] * m[0][3] * m[3][2] + m[3][1] * m[0][2] * m[1][3] - m[3][1] * m[0][3] * m[1][2];
  inv[1][2] = -m[0][0] * m[1][2] * m[3][3] + m[0][0] * m[1][3] * m[3][2] + m[1][0] * m[0][2] * m[3][3] - m[1][0] * m[0][3] * m[3][2] - m[3][0] * m[0][2] * m[1][3] + m[3][0] * m[0][3] * m[1][2];
  inv[2][2] =  m[0][0] * m[1][1] * m[3][3] - m[0][0] * m[1][3] * m[3][1] - m[1][0] * m[0][1] * m[3][3] + m[1][0] * m[0][3] * m[3][1] + m[3][0] * m[0][1] * m[1][3] - m[3][0] * m[0][3] * m[1][1];
  inv[3][2] = -m[0][0] * m[1][1] * m[3][2] + m[0][0] * m[1][2] * m[3][1] + m[1][0] * m[0][1] * m[3][2] - m[1][0] * m[0][2] * m[3][1] - m[3][0] * m[0][1] * m[1][2] + m[3][0] * m[0][2] * m[1][1];
  inv[0][3] = -m[0][1] * m[1][2] * m[2][3] + m[0][1] * m[1][3] * m[2][2] + m[1][1] * m[0][2] * m[2][3] - m[1][1] * m[0][3] * m[2][2] - m[2][1] * m[0][2] * m[1][3] + m[2][1] * m[0][3] * m[1][2];
  inv[1][3] =  m[0][0] * m[1][2] * m[2][3] - m[0][0] * m[1][3] * m[2][2] - m[1][0] * m[0][2] * m[2][3] + m[1][0] * m[0][3] * m[2][2] + m[2][0] * m[0][2] * m[1][3] - m[2][0] * m[0][3] * m[1][2];
  inv[2][3] = -m[0][0] * m[1][1] * m[2][3] + m[0][0] * m[1][3] * m[2][1] + m[1][0] * m[0][1] * m[2][3] - m[1][0] * m[0][3] * m[2][1] - m[2][0] * m[0][1] * m[1][3] + m[2][0] * m[0][3] * m[1][1];
  inv[3][3] =  m[0][0] * m[1][1] * m[2][2] - m[0][0] * m[1][2] * m[2][1] - m[1][0] * m[0][1] * m[2][2] + m[1][0] * m[0][2] * m[2][1] + m[2][0] * m[0][1] * m[1][2] - m[2][0] * m[0][2] * m[1][1];

  double det =  m[0][0] * inv[0][0] + m[0][1] * inv[1][0] + m[0][2] * inv[2][0] + m[0][3] * inv[3][0];

  if (fabs(det) < TOL) return Matrix();

  return inv / det;
} 

/*************************** TRANSFORM MATRICES ******************************/

inline Matrix ScaleMatrix(double sx, double sy, double sz) {
  return Matrix(
    sx,  0,  0, 0,
     0, sy,  0, 0,
     0,  0, sz, 0,
     0,  0,  0, 1
  );
}

inline Matrix TranslateMatrix(double tx, double ty, double tz) {
  return Matrix(
    1, 0, 0, tx,
    0, 1, 0, ty,
    0, 0, 1, tz,
    0, 0, 0,  1
  );
}

inline Matrix RotateXMatrix(double a) {
  return Matrix(
    1,      0,       0, 0,
    0, cos(a), -sin(a), 0,
    0, sin(a),  cos(a), 0,
    0,      0,       0, 1
  );
}

inline Matrix RotateYMatrix(double a) {
  return Matrix(
     cos(a), 0, sin(a), 0,
          0, 1,      0, 0,
    -sin(a), 0, cos(a), 0,
          0, 0,      0, 1
  );
}

inline Matrix RotateZMatrix(double a) {
  return Matrix(
    cos(a), -sin(a), 0, 0,
    sin(a),  cos(a), 0, 0,
         0,       0, 1, 0,
         0,       0, 0, 1
  );
}

inline Matrix getRotationMatrix(const Vec& n) {
  double theta = atan2(n.y, n.x);
  double phi = acos(n.z);
  return RotateZMatrix(theta) * RotateYMatrix(phi);
}

// Align vector `a` from +Z hemisphere to hemisphere with normal `n`
inline Vec alignTo(const Vec& a, const Vec& n) {
  return getRotationMatrix(n) * a;
}

/***************************** INTERPOLATION *********************************/

template<typename T>
inline T lerp(double fac, const T a, const T b) {
  return a + fac * (b - a);
}

template<typename T>
inline T inverseLerp(const T s, double sMin, double sMax) {
  return (s - sMin) / (sMax - sMin);
}

template<typename T>
inline T bilerp(double facX, double facY, const T xy, const T Xy,
                                          const T xY, const T XY) {
  return lerp(facY, lerp(facX, xy, Xy), lerp(facX, xY, XY));
}

template<typename T>
inline T trilerp(double facX, double facY, double facZ, 
                 const T xyz, const T Xyz, const T xYz, const T XYz,
                 const T xyZ, const T XyZ, const T xYZ, const T XYZ) {
  return lerp(facZ, bilerp(facX, facY, xyz, Xyz, xYz, XYz), 
                    bilerp(facX, facY, xyZ, XyZ, xYZ, XYZ));
}

template<typename T>
// Input: x ∈ [0,1], output: Faded value ∈ [0,1]
inline T fade(const T x) {
  return x * x * x * (x * (x * 6 - 15) + 10);
}

// Map value(s) from [sMin-sMax] to the new range [tMin-tMax]
template<typename T>
inline T map(const T a, double sMin=0 , double sMax=1, 
                        double tMin=-1, double tMax=1) {
  double ratio = (tMax - tMin) / (sMax - sMin);
  return (a - sMin) * ratio + tMin; 
}

inline double luminance(const Colour& c) {
  return 0.2126 * c.r + 0.7152 * c.g + 0.0722 * c.b;
}

/***************************** QUADRATIC SOLVING *****************************/

inline bool solveQuadratic(double a, double b, double c,
                           double *l1, double *l2) {
  double disc = b*b - 4*a*c;
  if (disc < 0) return false; // No solutions
  double root = sqrt(disc);
  *l1 = (-b - root) / (2 * a);
  *l2 = (-b + root) / (2 * a);
  return true;
}

#endif // __RT_MATH_H__ 
