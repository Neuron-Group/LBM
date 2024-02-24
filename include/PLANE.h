
/*
* PLANE 类型用于存储关于场景和计算域的基本信息，提供少量的方法
* 它应当拥有属性：
*   计算域的大小
*   密度矩阵
*   速度矩阵
*   障碍矩阵
*   速度源矩阵
*   速度源矩阵对应的源速度
*   密度源矩阵
*   密度源矩阵对应的源密度
* 它应当拥有如下方法：
*   获取数组坐标的方法
*   构造函数
*   初始化方法
*   提供计算域的大小
*   提供密度矩阵的值
*   提供速度矩阵的值
*   某个位置是否是障碍物
*   某个位置是否是速度源
*     如果是，应当可以查询它的速度
*   某个位置是否是密度源
*     如果是，应当可以查询它的密度
*   修改密度矩阵
*   修改速度矩阵
*   存储计算结果
*
*   更新函数，会被主程序调用以实现瞬态仿真
*/

#ifndef __PLANE_H__
#define __PLANE_H__

#include "bits/stdc++.h"
#include "POINT.h"

// for PLANE1
#include "Cspline.h"

#define D0 1.0
#define U0 0.15
#pragma GCC optimize(3,"Ofast","inline")
class PLANE{
  int sizeX, sizeY;
  float * rho;
  POINT * vel;
  bool * barr;
  bool * issource_vel;
  POINT * source_vel;
  bool * issource_rho;
  float * source_rho;

  // for PLANE1
  CSpline * sp0;
  CSpline * sp1;

public:
  int idx(IDX pos);
  PLANE();
  void INIT();
  IDX size();
  float Rho(IDX pos);
  VECTOR V(IDX pos);
  bool isBarr(IDX pos);
  bool isSourceV(IDX pos);
  VECTOR SourceV(IDX pos);
  bool isSourceRho(IDX pos);
  float sourceRho(IDX pos);
  void mRho(IDX pos, float value);
  void mV(IDX pos, VECTOR value);
  void SAVE();
  void UPDATE();
};

#endif //__PLANE_H__

