/**
 * Math library for ray-tracing. Includes:
 *    - Common utility functions: min(), max(), clamp01()
 *    - Global Constants: PI, TAU, TOL
 *    - Random number generation: rand01()
 *    - Vector math definitions: Vec, dot(), cross(), ...
 *    - Matrix math definitions: Matrix, mat-vec / mat-mat mult, 4x4 invert
 *    - Transformation matrices: Scale, Translate, Rotate X/Y/Z
 *    - Quadratic Solving: solveQuadratic()
 */

#ifndef __RT_MATH_H__
#define __RT_MATH_H__

#include <iostream>
#include <math.h>
#include <stdlib.h>

/**************************** GLOBAL CONSTANTS *******************************/

#define PI 3.141592653
#define TAU 6.283185307
#define TOL (10e-6)


/**************************** COMMON FUNCTIONS *******************************/

double min(double a, double b);
double max(double a, double b);
double clamp01(double a);

/************************** RANDOM NUMBERS ***********************************/

double rand01(void);

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

std::ostream &operator<<(std::ostream &os, const Vec& t);

Vec operator+(const Vec& v1, const Vec& v2);
Vec operator+(const Vec& v, const double t);
Vec operator+(const double t, const Vec& v);

Vec operator-(const Vec& v1);
Vec operator-(const Vec& v1, const Vec& v2);
Vec operator-(const Vec& v, const double t);
Vec operator-(const double t, const Vec& v);

Vec operator*(const double t, const Vec& v);
Vec operator*(const Vec& v, const double t);

Vec operator/(const double t, const Vec& v);
Vec operator/(const Vec& v, const double t);

double length(const Vec& a);
Vec norm(const Vec& a);
Vec mult(const Vec& a, const Vec& b);

double dot(const Vec& a, const Vec& b);
Vec cross(const Vec& a, const Vec& b); 

Vec& operator+=(Vec &a, const Vec& b);
Vec& operator+=(Vec &a, const double& b);

Vec& operator-=(Vec &a, const Vec& b);
Vec& operator-=(Vec &a, const double b);

Vec& operator*=(Vec &a, const double b);
Vec& operator/=(Vec &a, const double b);


Vec clamp01(const Vec& a);
Vec randomVector();
Vec randomVectorHemisphere(Vec n=Vec(0,0,1));

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

Matrix operator+(const Matrix& a, const Matrix& m2);
Matrix operator+(const Matrix& a, double d);
Matrix operator+(double d, const Matrix& a);


Matrix operator-(const Matrix& a, const Matrix& b);
Matrix operator-(const Matrix& a, double d);
Matrix operator-(double d, const Matrix& a);

Matrix operator*(const Matrix& a, double t);
Matrix operator*(double t, const Matrix& a);

Matrix operator/(const Matrix& a, double t);
Matrix operator/(double t, const Matrix& a);

Matrix& operator+=(Matrix &a, const Matrix b);
Matrix& operator+=(Matrix &a, double b);

Matrix& operator-=(Matrix &a, const Matrix b);
Matrix& operator-=(Matrix &a, double b);

Matrix& operator*=(Matrix &a, double b);
Matrix& operator/=(Matrix &a, double b);

Matrix transpose(const Matrix &a);

/************* MATRIX-VEC / MATRIX-MATRIX MULTIPLICATION *********************/

Matrix operator*(const Matrix& a, const Matrix& b);
// NOTE: Assuming w component is always 0, no homogenous-normalizing here.
Vec operator*(const Matrix& m, const Vec& v);
// This transforms the Vector **WITHOUT** the translation component  
Vec operator%(const Matrix& m, const Vec& v);
std::ostream& operator<<(std::ostream& os, const Matrix& m);

/*************************** MATRIX INVERSION ********************************/

// Source: https://stackoverflow.com/questions/1148309
Matrix invert(const Matrix &m);

/*************************** TRANSFORM MATRICES ******************************/

Matrix ScaleMatrix(double sx, double sy, double sz);
Matrix TranslateMatrix(double tx, double ty, double tz);
Matrix RotateXMatrix(double a);
Matrix RotateYMatrix(double a);
Matrix RotateZMatrix(double a);

/***************************** QUADRATIC SOLVING *****************************/

bool solveQuadratic(double a, double b, double c, double *l1, double *l2);


#endif // __RT_MATH_H__ 
