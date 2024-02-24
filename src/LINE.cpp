
#include "LINE.h"
#include "POINT.h"
#include "bits/stdc++.h"

LINE::LINE(POINT A_, POINT B_) {
  this->A = A_;
  this->B = B_;
  this->isCutA = false;
  this->isCutB = false;
  this->eps = EPS;
}

LINE::LINE(LINE &b) {
  this->A = b.a();
  this->B = b.b();
  this->isCutA = b.isCuta();
  this->isCutB = b.isCutb();
  this->eps = b.printEps();
}

void LINE::cutA() { this->isCutA = true; }

void LINE::cutB() { this->isCutB = true; }

void LINE::uncutA() { this->isCutA = false; }

void LINE::uncutB() { this->isCutB = false; }

LINE LINE::uncut() {
  LINE line(this->A, this->B);
  line.eps = this->eps;
  return line;
}

void LINE::setA(POINT A_) { this->A = A_; }

void LINE::setB(POINT B_) { this->B = B_; }

void LINE::set(POINT A_, POINT B_) {
  this->A = A_;
  this->B = B_;
}

void LINE::setEPS(float eps_) { this->eps = eps_; }

POINT LINE::a() { return this->A; }

POINT LINE::b() { return this->B; }

bool LINE::isCuta() { return this->isCutA; }

bool LINE::isCutb() { return this->isCutB; }

float LINE::printEps() { return this->eps; }

float LINE::abs() { return (this->B - this->A).abs(); }

bool LINE::isPL(POINT &b) {
  bool ans = false;
  POINT Ab = b - this->A;
  POINT Bb = b - this->B;
  POINT AB = this->B - this->A;
  if (!dcmp(Ab ^ Bb, this->eps)) {
    ans = true;
  }

  if (this->isCutA && ((dcmp(Ab * AB, this->eps)) < 0)) {
    ans = false;
  }

  if (this->isCutB && ((dcmp(AB * Bb, this->eps)) > 0)) {
    ans = false;
  }

  return ans;
}

bool LINE::operator|(POINT &b) { return this->isPL(b); }

float LINE::disPL(POINT &b) {
  VECTOR Ab = b - this->A;
  VECTOR Bb = b - this->B;
  VECTOR AB = this->B - this->A;

  if (this->isCutA && (dcmp(Ab * AB, this->eps) < 0)) {
    return Ab.abs();
  }

  if (this->isCutB && (dcmp(AB * Bb, this->eps) > 0)) {
    return Bb.abs();
  }

  if (dcmp(AB.abs(), this->eps) == 0) {
    return (Ab.abs() + Bb.abs()) / 2.;
  }

  return Abs((Ab ^ Bb) / AB.abs());
}

float LINE::operator^(POINT &b) { return this->disPL(b); }

POINT LINE::footPoint(POINT &b) {
  VECTOR Ab = b - this->A;
  VECTOR Bb = b - this->B;
  VECTOR AB = this->B - this->A;

  if (dcmp(AB.abs(), this->eps) == 0) {
    return (this->A + this->B) / 2.;
  }

  float len1 = Ab * AB / AB.abs();
  float len2 = -1.0 * (Bb * AB) / AB.abs();

  POINT T = (AB * (len1 / (len1 + len2)));
  return this->A + T;
}

POINT LINE::symmetryLine(POINT &b) {
  POINT pointb = b;
  POINT point = footPoint(b) * 2.;
  return pointb + point;
}

POINT LINE::crossLL(LINE &b) {
  POINT C = b.a();
  POINT D = b.b();
  VECTOR AB = this->B - this->A;
  VECTOR CD = D - C;
  VECTOR CA = this->A - C;
  if (dcmp(AB ^ CD, this->eps) == 0) {
    return this->A;
  }

  VECTOR AF = AB * ((CD ^ CA) / (AB ^ CD));

  return this->A + AF;
}

POINT LINE::operator^(LINE &b) { return this->crossLL(b); }

bool LINE::isCrossLL(LINE &b) {
  POINT C = b.a();
  POINT D = b.b();
  VECTOR AB = this->B - this->A;
  VECTOR CD = D - C;
  VECTOR CA = this->A - C;
  VECTOR CB = this->B - C;
  if (dcmp(AB ^ CD, this->eps) == 0) {
    if (dcmp(CA ^ CB, this->eps) == 0) {
      return true;
    } else {
      return false;
    }
  } else {
    POINT crossPoint = this->crossLL(b);
    if (this->isPL(crossPoint) && b.isPL(crossPoint)) {
      return true;
    } else {
      return false;
    }
  }
}

bool LINE::operator|(LINE &b) { return this->isCrossLL(b); }

POINT crossLL(LINE &a, LINE &b) { return a.crossLL(b); }

bool isCrossLL(LINE &a, LINE &b) { return a.isCrossLL(b); }

float abs(LINE &a) { return a.abs(); }
