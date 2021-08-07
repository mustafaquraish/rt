#pragma once
// clang-format off
#include <iomanip>

#include <core/vec.h>

/************ 2D Homogenous Transformation Matrix *****************/

class Matrix3 {
 public:
  Matrix3() : m_vals{{1,0,0},{0,1,0},{0,0,1}} {}
  Matrix3(float v) : m_vals{{v,v,v},{v,v,v},{v,v,v}} {}
  Matrix3(float m00, float m01, float m02,
          float m10, float m11, float m12,
          float m20, float m21, float m22)
    : m_vals{{m00,m01,m02},
             {m10,m11,m12},
             {m20,m21,m22}} {}

  float *operator[](size_t i) { return m_vals[i]; }
  float const *operator[](size_t i) const { return m_vals[i]; };

  void apply(const Matrix3 &m);
 private:
  float m_vals[3][3];
};

Matrix3 invert(const Matrix3 &m);

inline Matrix3 operator+(const Matrix3 &a, const Matrix3 &m2) {
  Matrix3 m = Matrix3();
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) 
      m[i][j] = a[i][j] + m2[i][j];
  return m;
}
inline Matrix3 operator+(const Matrix3 &a, float d) { return a + Matrix3(d); }
inline Matrix3 operator+(float d, const Matrix3 &a) { return a + Matrix3(d); }

inline Matrix3 operator-(const Matrix3 &a, const Matrix3 &b) {
  Matrix3 m = Matrix3();
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) 
      m[i][j] = a[i][j] - b[i][j];
  return m;
}
inline Matrix3 operator-(const Matrix3 &a, float d) { return a - Matrix3(d); }
inline Matrix3 operator-(float d, const Matrix3 &a) { return Matrix3(d) - a; }

inline Matrix3 operator*(const Matrix3 &a, float t) {
  Matrix3 m = Matrix3();
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      m[i][j] = a[i][j] * t;
  return m;
}
inline Matrix3 operator*(float t, const Matrix3 &a) { return a * t; }
inline Matrix3 operator/(const Matrix3 &a, float t) { return a * (1 / t); }

inline Matrix3 &operator+=(Matrix3 &a, const Matrix3 b) { return a = a + b; }
inline Matrix3 &operator+=(Matrix3 &a, float b) { return a = a + b; }

inline Matrix3 &operator-=(Matrix3 &a, const Matrix3 b) { return a = a - b; }
inline Matrix3 &operator-=(Matrix3 &a, float b) { return a = a - b; }

inline Matrix3 &operator*=(Matrix3 &a, float b) { return a = a * b; }
inline Matrix3 &operator/=(Matrix3 &a, float b) { return a = a / b; }

inline Matrix3 transpose(const Matrix3 &a) {
  Matrix3 m = Matrix3();
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++)
      m[i][j] = a[j][i];
  return m;
}

inline Matrix3 operator*(const Matrix3 &a, const Matrix3 &b) {
  Matrix3 m = Matrix3();
  for (int i = 0; i < 3; i++)
    for (int j = 0; j < 3; j++) {
      m[i][j] = a[i][0] * b[0][j];
      m[i][j] += a[i][1] * b[1][j];
      m[i][j] += a[i][2] * b[2][j];
    }
  return m;
}

// NOTE: Mind the division by the homogenous coordinate!
inline Vec2 operator*(const Matrix3 &m, const Vec2 &v) {
  return Vec2(
       m[0][0] * v.x + m[0][1] * v.y + m[0][2],
       m[1][0] * v.x + m[1][1] * v.y + m[1][2]
  ) / (m[2][0] * v.x + m[2][1] * v.y + m[2][2]);
}

// This transforms the Vector **WITHOUT** the translation component  
inline Vec2 operator%(const Matrix3& m, const Vec2& v) {
  return Vec2(
    m[0][0] * v.x + m[0][1] * v.y,
    m[1][0] * v.x + m[1][1] * v.y
  );
}

inline void Matrix3::apply(const Matrix3 &m) { *this = m * (*this); }
inline Matrix3 &operator*=(Matrix3 &a, const Matrix3 &b) { return a = a * b; }

inline std::ostream &operator<<(std::ostream &os, const Matrix3 &m) {
  os << "[";
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      os << std::setprecision(3) << std::setfill('0') << m[i][j] << ", ";
    }
    if (i != 3 - 1) os << "\n ";
  }
  os << "]";
  return os;
}

inline void print_matrix(const Matrix3 &m) {
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) printf("%2.3e, ", m[i][j]);
    printf("\n");
  }
}

inline Matrix3 ScaleMatrix(float sx, float sy) {
  return Matrix3(
    sx,  0, 0,
     0, sy, 0,
     0,  0, 1
  );
}

inline Matrix3 TranslateMatrix(float tx, float ty) {
  return Matrix3(
    1, 0, tx,
    0, 1, ty,
    0, 0,  1
  );
}

inline Matrix3 RotateMatrix(float a) {
  return Matrix3(
    cos(a), -sin(a), 0,
    sin(a),  cos(a), 0,
         0,       0, 1
  );
}

inline Matrix3 get_rotation_matrix(const Vec2& n) {
  float theta = atan2(n.y, n.x);
  return RotateMatrix(theta);
}

// Align vector `a` from +X semicircle to semicircle with normal `n`
inline Vec2 align_to(const Vec2& a, const Vec2& n) {
  return get_rotation_matrix(n) * a;
}

/************ 3D Homogenous Transformation Matrix *****************/

class Matrix4 {
 public:
  Matrix4() : m_vals{{1,0,0,0},{0,1,0,0},{0,0,1,0},{0,0,0,1}} {}
  Matrix4(float v) : m_vals{{v,v,v,v},{v,v,v,v},{v,v,v,v},{v,v,v,v}} {}
  Matrix4(float m00, float m01, float m02, float m03,
          float m10, float m11, float m12, float m13,
          float m20, float m21, float m22, float m23, 
          float m30, float m31, float m32, float m33)
    : m_vals{{m00,m01,m02,m03},
             {m10,m11,m12,m13},
             {m20,m21,m22,m23},
             {m30,m31,m32,m33}} {}

  float *operator[](size_t i) { return m_vals[i]; }
  float const *operator[](size_t i) const { return m_vals[i]; };

  void apply(const Matrix4 &m);
 private:
  float m_vals[4][4];
};

Matrix4 invert(const Matrix4 &m);

inline Matrix4 operator+(const Matrix4 &a, const Matrix4 &m2) {
  Matrix4 m = Matrix4();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
      m[i][j] = a[i][j] + m2[i][j];
  return m;
}
inline Matrix4 operator+(const Matrix4 &a, float d) { return a + Matrix4(d); }
inline Matrix4 operator+(float d, const Matrix4 &a) { return a + Matrix4(d); }

inline Matrix4 operator-(const Matrix4 &a, const Matrix4 &b) {
  Matrix4 m = Matrix4();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) 
      m[i][j] = a[i][j] - b[i][j];
  return m;
}
inline Matrix4 operator-(const Matrix4 &a, float d) { return a - Matrix4(d); }
inline Matrix4 operator-(float d, const Matrix4 &a) { return Matrix4(d) - a; }

inline Matrix4 operator*(const Matrix4 &a, float t) {
  Matrix4 m = Matrix4();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      m[i][j] = a[i][j] * t;
  return m;
}
inline Matrix4 operator*(float t, const Matrix4 &a) { return a * t; }
inline Matrix4 operator/(const Matrix4 &a, float t) { return a * (1 / t); }

inline Matrix4 &operator+=(Matrix4 &a, const Matrix4 b) { return a = a + b; }
inline Matrix4 &operator+=(Matrix4 &a, float b) { return a = a + b; }

inline Matrix4 &operator-=(Matrix4 &a, const Matrix4 b) { return a = a - b; }
inline Matrix4 &operator-=(Matrix4 &a, float b) { return a = a - b; }

inline Matrix4 &operator*=(Matrix4 &a, float b) { return a = a * b; }
inline Matrix4 &operator/=(Matrix4 &a, float b) { return a = a / b; }

inline Matrix4 transpose(const Matrix4 &a) {
  Matrix4 m = Matrix4();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++)
      m[i][j] = a[j][i];
  return m;
}

inline Matrix4 operator*(const Matrix4& a, const Matrix4& b) {
  Matrix4 m = Matrix4();
  for (int i = 0; i < 4; i++)
    for (int j = 0; j < 4; j++) {
      m[i][j] = a[i][0] * b[0][j];
      m[i][j] += a[i][1] * b[1][j];
      m[i][j] += a[i][2] * b[2][j];
      m[i][j] += a[i][3] * b[3][j];
    }
  return m;
}

// NOTE: Mind the division by the homogenous coordinate!
inline Vec3 operator*(const Matrix4 &m, const Vec3 &v) {
  return Vec3(
       m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z + m[0][3],
       m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z + m[1][3],
       m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z + m[2][3]
  );
}

// This transforms the Vector **WITHOUT** the translation component  
inline Vec3 operator%(const Matrix4& m, const Vec3& v) {
  return Vec3(
    m[0][0] * v.x + m[0][1] * v.y + m[0][2] * v.z,
    m[1][0] * v.x + m[1][1] * v.y + m[1][2] * v.z,
    m[2][0] * v.x + m[2][1] * v.y + m[2][2] * v.z
  );
}

inline void Matrix4::apply(const Matrix4 &m) { *this = m * (*this); }
inline Matrix4 &operator*=(Matrix4 &a, const Matrix4 &b) { return a = a * b; }

inline std::ostream &operator<<(std::ostream &os, const Matrix4 &m) {
  os << "[";
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      os << std::setprecision(4) << std::setfill('0') << m[i][j] << ", ";
    }
    if (i != 4 - 1) os << "\n ";
  }
  os << "]";
  return os;
}

inline void print_matrix(const Matrix4 &m) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) 
      printf("%2.3e, ", m[i][j]);
    printf("\n");
  }
}

inline Matrix4 ScaleMatrix(float sx, float sy, float sz) {
  return Matrix4(
    sx,  0,  0, 0,
     0, sy,  0, 0,
     0,  0, sz, 0,
     0,  0,  0, 1
  );
}

inline Matrix4 TranslateMatrix(float tx, float ty, float tz) {
  return Matrix4(
    1, 0, 0, tx,
    0, 1, 0, ty,
    0, 0, 1, tz,
    0, 0, 0,  1
  );
}

inline Matrix4 RotateXMatrix(float a) {
  return Matrix4(
    1,      0,       0, 0,
    0, cos(a), -sin(a), 0,
    0, sin(a),  cos(a), 0,
    0,      0,       0, 1
  );
}

inline Matrix4 RotateYMatrix(float a) {
  return Matrix4(
     cos(a), 0, sin(a), 0,
          0, 1,      0, 0,
    -sin(a), 0, cos(a), 0,
          0, 0,      0, 1
  );
}

inline Matrix4 RotateZMatrix(float a) {
  return Matrix4(
    cos(a), -sin(a), 0, 0,
    sin(a),  cos(a), 0, 0,
         0,       0, 1, 0,
         0,       0, 0, 1
  );
}

inline Matrix4 get_rotation_matrix(const Vec3 &n) {
  float theta = atan2(n.y, n.x);
  float phi = acos(n.z);
  return RotateZMatrix(theta) * RotateYMatrix(phi);
}

// Align vector `a` from +X semicircle to semicircle with normal `n`
inline Vec3 align_to(const Vec3& a, const Vec3& n) {
  return get_rotation_matrix(n) * a;
}