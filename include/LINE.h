#ifndef __LINE_H__
#define __LINE_H__

#include "POINT.h"
#include "bits/stdc++.h"

class LINE{
  POINT A, B;
  bool isCutA, isCutB; // 是否在端点处裁剪
  float eps;  // 精度
  
public:
  LINE(POINT A_ = {0.,0.}, POINT B_ = {0.,0.}); // 构造函数
  LINE(LINE &b); // 拷贝函数
  void cutA(); // 在A处裁剪
  void uncutA(); // 在A处去除裁剪
  void cutB(); // 在B处裁剪
  void uncutB(); // 在B处去除裁剪
  LINE uncut(); // 输出去除裁剪的对象
  void setA(POINT A_); // 重置点A
  void setB(POINT B_); // 重置点B
  void set(POINT A_, POINT B_); // 重置两个点
  void setEPS(float eps_); // 设置精度
  POINT a(); // 输出点A
  POINT b(); // 输出点B
  bool isCuta(); // 输出点A的裁剪情况
  bool isCutb(); // 输出点B的裁剪情况
  float printEps(); // 输出精度
  float abs(); // 模长
  bool isPL(POINT &b); // 点是否在线段上
  bool operator | (POINT &b); // isPL
  float disPL(POINT &b); // 点与直线的距离
  float operator ^ (POINT &b); // disPL
  POINT footPoint(POINT &b); // 垂足
  POINT symmetryLine(POINT &b); // 对称点
  POINT crossLL(LINE &b); // 两直线的交点（无论是否裁剪）
  bool isCrossLL(LINE &b); // 判断是否相交
  POINT operator ^ (LINE &b); // crossLL
  bool operator | (LINE &b); // isCrossLL
};

POINT crossLL(LINE &a, LINE &b);
bool isCrossLL(LINE &a, LINE &b);
float abs(LINE &a);

#endif //__LINE_H__
