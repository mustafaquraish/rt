#include "core/math.h"
#include <iomanip>      // std::setprecision

/**************************** COMMON FUNCTIONS *******************************/

double min(double a, double b) { return (a < b) ? a : b; }
double max(double a, double b) { return (a > b) ? a : b; }
double clamp01(double a) { return min(1, max(a, 0)); }

/************************** RANDOM NUMBERS ***********************************/

double rand01(void) {
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

std::ostream &operator<<(std::ostream &os, const Vec& t) { 
  os << t.x << " " << t.y << " " << t.z; 
  return os; 
}

Vec operator+(const Vec& v1, const Vec& v2) { return Vec(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z); }
Vec operator+(const Vec& v, const double t) { return Vec(v.x + t, v.y + t, v.z + t); }
Vec operator+(const double t, const Vec& v) { return Vec(v.x + t, v.y + t, v.z + t); }

Vec operator-(const Vec& v1) { return Vec(-v1.x, -v1.y, -v1.z); }
Vec operator-(const Vec& v1, const Vec& v2) { return Vec(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z); }
Vec operator-(const Vec& v, const double t) { return Vec(v.x - t, v.y - t, v.z - t); }
Vec operator-(const double t, const Vec& v) { return Vec(v.x - t, v.y - t, v.z - t); }

Vec operator*(const double t, const Vec& v) { return Vec(t * v.x, t * v.y, t * v.z); }
Vec operator*(const Vec& v, const double t) { return Vec(t * v.x, t * v.y, t * v.z); }

Vec operator/(const double t, const Vec& v) { return Vec(v.x / t, v.y / t, v.z / t); }
Vec operator/(const Vec& v, const double t) { return Vec(v.x / t, v.y / t, v.z / t); }

double length(const Vec& a) { return sqrt(a.x * a.x + a.y * a.y + a.z * a.z); }
Vec norm(const Vec& a) { return a * (1 / sqrt(a.x * a.x + a.y * a.y + a.z * a.z)); }
Vec mult(const Vec& a, const Vec& b) { return Vec(a.x * b.x, a.y * b.y, a.z * b.z); }

double dot(const Vec& a, const Vec& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
Vec cross(const Vec& a, const Vec& b) { 
  return Vec( a.y * b.z - a.z * b.y, 
             -a.x * b.z + a.z * b.x, 
              a.x * b.y - a.y * b.x); 
}

Vec& operator+=(Vec &a, const Vec& b) { return a = a + b; }
Vec& operator+=(Vec &a, const double& b) { return a = a + b; }

Vec& operator-=(Vec &a, const Vec& b) { return a = a - b; }
Vec& operator-=(Vec &a, const double b) { return a = a - b; }

Vec& operator*=(Vec &a, const double b) { return a = a * b; }
Vec& operator/=(Vec &a, const double b) { return a = a / b; }


Vec clamp01(const Vec& a) {
  return Vec(min(1, max(a.x, 0)),
             min(1, max(a.y, 0)),
             min(1, max(a.z, 0)));
}

Vec randomVector() {
  return norm(2 * Vec(rand01(), rand01(), rand01()) - 1);
}

Vec randomVectorHemisphere(const Vec& n=Vec(0,0,1)) {
  Vec canon = randomVector();
  while (dot(canon, n) < 0) {
    canon = randomVector();
  }
  return canon;
}


Matrix operator+(const Matrix& a, const Matrix& m2) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
      m[i][j] = a[i][j] + m2[i][j];
  return m;
}
Matrix operator+(const Matrix& a, double d) { return a + Matrix(d); };
Matrix operator+(double d, const Matrix& a) { return a + Matrix(d); };


Matrix operator-(const Matrix& a, const Matrix& b) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
      m[i][j] = a[i][j] - b[i][j];
  return m;
}
Matrix operator-(const Matrix& a, double d) { return a - Matrix(d); };
Matrix operator-(double d, const Matrix& a) { return a - Matrix(d); };

Matrix operator*(const Matrix& a, double t) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      m[i][j] = a[i][j] * t;
    }
  return m;
}
Matrix operator*(double t, const Matrix& a) { return a * t; }

Matrix operator/(const Matrix& a, double t) { return a * (1 / t); }
Matrix operator/(double t, const Matrix& a) { return a * (1 / t); }

Matrix& operator+=(Matrix &a, const Matrix b) { return a = a + b; }
Matrix& operator+=(Matrix &a, double b) { return a = a + b; }

Matrix& operator-=(Matrix &a, const Matrix b) { return a = a - b; }
Matrix& operator-=(Matrix &a, double b) { return a = a - b; }

Matrix& operator*=(Matrix &a, double b) { return a = a * b; }
Matrix& operator/=(Matrix &a, double b) { return a = a / b; }

Matrix transpose(const Matrix &a) {
  Matrix m = Matrix();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      m[i][j] = a[j][i];
    }
  return m;
}

/************* MATRIX-VEC / MATRIX-MATRIX MULTIPLICATION *********************/

Matrix operator*(const Matrix& a, const Matrix& b) {
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
Vec operator*(const Matrix& m, const Vec& v) {
  return Vec(
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]
  );
}

// This transforms the Vector **WITHOUT** the translation component  
Vec operator%(const Matrix& m, const Vec& v) {
  return Vec(
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
  );
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
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

/*************************** MATRIX INVERSION ********************************/

// Source: https://stackoverflow.com/questions/1148309
Matrix invert(const Matrix &m) {
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

Matrix ScaleMatrix(double sx, double sy, double sz) {
  return Matrix(
    sx,  0,  0, 0,
     0, sy,  0, 0,
     0,  0, sz, 0,
     0,  0,  0, 1
  );
}

Matrix TranslateMatrix(double tx, double ty, double tz) {
  return Matrix(
    1, 0, 0, tx,
    0, 1, 0, ty,
    0, 0, 1, tz,
    0, 0, 0,  1
  );
}

Matrix RotateXMatrix(double a) {
  return Matrix(
    1,      0,       0, 0,
    0, cos(a), -sin(a), 0,
    0, sin(a),  cos(a), 0,
    0,      0,       0, 1
  );
}

Matrix RotateYMatrix(double a) {
  return Matrix(
     cos(a), 0, sin(a), 0,
          0, 1,      0, 0,
    -sin(a), 0, cos(a), 0,
          0, 0,      0, 1
  );
}

Matrix RotateZMatrix(double a) {
  return Matrix(
    cos(a), -sin(a), 0, 0,
    sin(a),  cos(a), 0, 0,
         0,       0, 1, 0,
         0,       0, 0, 1
  );
}

/***************************** QUADRATIC SOLVING *****************************/

bool solveQuadratic(double a, double b, double c,
                           double *l1, double *l2) {
  double disc = b*b - 4*a*c;
  if (disc < TOL) return false; // No solutions

  double root = sqrt(disc);
  *l1 = (-b - root) / (2 * a);
  *l2 = (-b + root) / (2 * a);
  return true;
}