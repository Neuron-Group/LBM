
#ifndef __POINT_H__
#define __POINT_H__

#define EPS 1e-6
#define VECTOR POINT
#pragma GCC optimize(3, "Ofast", "inline")
// using namespace std;

int dcmp(float x, float eps = EPS);
float Abs(float x);

class IDX;

class POINT {
  float X, Y;

public:
  void read();
  POINT(float x = 0., float y = 0.);
  POINT(const POINT &b);
  const float x();
  const float y();
  void setX(float x);
  void setY(float y);
  void set(float x, float y);
  bool operator==(POINT &b);
  POINT operator+(POINT &b);
  POINT operator-(POINT &b);
  float abs();               // 模长
  POINT operator*(float x);  // 数乘
  POINT operator/(float x);  // 数乘（除以常数）
  float operator*(POINT &b); // 点积
  float operator^(POINT &b); // 叉积

  VECTOR turn_O(float theta);          // 向量旋转
  POINT turn_P(POINT &b, float theta); // 绕点旋转

  VECTOR symmetryVector(VECTOR v); // 关于向量对称
  POINT symmetryPoint(POINT p);    // 关于某一点对称

  IDX POINT2IDX();
  IDX FLOOR();
  POINT FRAC();

  POINT operator-(IDX &b);

  // bool operator == (POINT b);
  // POINT operator + (POINT b);
  // POINT operator - (POINT b);
  // float operator * (POINT b);
  // float operator ^ (POINT b);
  // POINT turn_P(POINT b, float theta);
};

// <
bool cmp(POINT a, POINT b);
bool cmp_x(POINT a, POINT b);
bool cmp_y(POINT a, POINT b);

float Dot(POINT &a, POINT &b); // 点积
float Cro(POINT &a, POINT &b); // 叉积

POINT operator*(float x, VECTOR &b); // 还是数乘
float abs(VECTOR &a);                // 还是模长

class IDX {
  int X;
  int Y;

public:
  int x();

  int y();

  IDX(int x_ = 0, int y_ = 0);
  IDX(IDX &b);

  void setX(int x_);
  void setY(int y_);
  void set(int x_, int y_);

  IDX operator+(const IDX &b);

  IDX operator-(const IDX &b);

  IDX operator*(const int b);

  POINT operator-(const POINT &b);

  POINT operator+(const POINT &b);

  POINT IDX2POINT();
};

#endif //__POINT_H__
