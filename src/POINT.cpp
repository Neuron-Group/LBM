
#include "POINT.h"
#include "bits/stdc++.h"
#define EPS 1e-6

using namespace std;

int dcmp(float x, float eps) { return x < -eps ? -1 : (x < eps ? 0 : 1); }

float Abs(float x) { return x * (float)dcmp(x); }

void POINT::read() {
  cin >> this->X;
  cin >> this->Y;
}
POINT::POINT(float x, float y) {
  this->X = x;
  this->Y = y;
}
POINT::POINT(const POINT &b) {
  this->X = b.X;
  this->Y = b.Y;
}
const float POINT::x() { return this->X; }
const float POINT::y() { return this->Y; }
void POINT::setX(float x) { this->X = x; }
void POINT::setY(float y) { this->Y = y; }
void POINT::set(float x, float y) {
  this->X = x;
  this->Y = y;
}
bool POINT::operator==(POINT &b) {
  return !dcmp(this->X - b.x()) && !dcmp(this->Y - b.y());
}
POINT POINT::operator+(POINT &b) { return POINT(x() + b.x(), y() + b.y()); }
POINT POINT::operator-(POINT &b) { return POINT(x() - b.x(), y() - b.y()); }
float POINT::abs() { return sqrt(X * X + Y * Y); }
POINT POINT::operator*(float x) { return POINT(this->X * x, this->Y * x); }
POINT POINT::operator/(float x) { return POINT(this->X / x, this->Y / x); }
float POINT::operator*(POINT &b) { return this->X * b.x() + this->Y * b.y(); }
float POINT::operator^(POINT &b) { return this->X * b.y() - this->Y * b.x(); }
VECTOR POINT::turn_O(float theta) {
  return POINT(this->X * cos(theta) + this->Y * sin(theta),
               -this->X * sin(theta) + this->Y * cos(theta));
}
POINT POINT::turn_P(POINT &b, float theta) {
  return POINT(
      (this->X - b.x()) * cos(theta) + (this->Y - b.y()) * sin(theta) + b.x(),
      -(this->X - b.x()) * sin(theta) + (this->Y - b.y()) * cos(theta) + b.y());
}

VECTOR POINT::symmetryVector(VECTOR v) {
  VECTOR x = *this;
  return (2 * (x * v) / v.abs() / v.abs()) * v - x;
}

POINT POINT::symmetryPoint(POINT p) {
  POINT x = *this;
  return (p - x) * 2. + x;
}

IDX POINT::POINT2IDX() { return IDX((int)round(this->X), (int)round(this->Y)); }

POINT POINT::operator-(IDX &b) {
  return POINT(this->X - (float)b.x(), this->Y - (float)b.y());
}

bool cmp_x(POINT a, POINT b) { return a.x() < b.x(); }
bool cmp_y(POINT a, POINT b) { return a.y() < b.y(); }
bool cmp(POINT a, POINT b) {
  if (a.x() == b.x()) {
    return a.y() < b.y();
  }
  return a.x() < b.x();
}

float Dot(POINT &a, POINT &b) { return a * b; }
float Cro(POINT &a, POINT &b) { return a ^ b; }

POINT operator*(float x, VECTOR &b) { return POINT(x * b.x(), x * b.y()); }

float abs(VECTOR &a) { return a.abs(); }

int IDX::x() { return this->X; }

int IDX::y() { return this->Y; }

IDX::IDX(int x_, int y_) {
  this->X = x_;
  this->Y = y_;
}

IDX::IDX(IDX &b) {
  this->X = b.X;
  this->Y = b.Y;
}

IDX IDX::operator+(const IDX &b) { return IDX(this->X + b.X, this->Y + b.Y); }

IDX IDX::operator-(const IDX &b) { return IDX(this->X - b.X, this->Y - b.Y); }

IDX IDX::operator*(const int b) { return IDX(this->X * b, this->Y * b); }

POINT IDX::operator-(const POINT &b) {
  POINT ans = b;
  ans.set((float)this->X - ans.x(), (float)this->Y - ans.y());
  return ans;
}

POINT IDX::operator+(const POINT &b) {
  POINT ans = b;
  ans.set((float)this->X + ans.x(), (float)this->Y + ans.y());
  return ans;
}

POINT IDX::IDX2POINT() {
  POINT ans((float)this->X, (float)this->Y);
  return ans;
}

IDX POINT::FLOOR() { return IDX((int)floor(this->X), (int)floor(this->Y)); }

POINT POINT::FRAC() {
  return POINT(this->X - (float)floor(this->X),
               this->Y - (float)floor(this->Y));
}

void IDX::setX(int x_) {
  this->X = x_;
  // return *this;
}

void IDX::setY(int y_) {
  this->Y = y_;
  // return *this;
}

void IDX::set(int x_, int y_) {
  this->X = x_;
  this->Y = y_;
}
