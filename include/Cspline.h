
#ifndef __CSPLINE_H__
#define __CSPLINE_H__

#include "bits/stdc++.h"
#include "POINT.h"

#define INF 2147483647

// 懒惰计算
// 插入节点时计算dy 和ddy
// $dy[i] = y_(i+1) - y_i$
// $ddy[i] = y_(i+2) - y_(i)$
//
// 计算 c_i：
// num_insertpoints() - c_i.size() <= i ?
// T:
//  c_i = num_insertpoints() - i - 1;
//  c_i+1 = num_insertpoints() - i;
// F:
//  while(num_insertpoints() - c_i.size() > i){
//    追赶法计算c_k
//    c_i.push_back(c_k)
//  }
//
// 插入计算：
// 在节点列表中插入节点
// 更新所有的差分数组
// 重新计算 $beta_(n-1)$
// 计算 $beta_(n)$
// 重新计算 $y_(n-1)$
// 计算 $y_n$
//
// 取值操作：
// 如果已经被插入新的节点
//  更新 num_points
//  清空 c_i
// 二分算法判断包含给定 $x$ 的区间 $[x_i, x_(i+1)]$
// 计算 c_i

class CSpline{
private:

  // 插值点的数量
  int num_points;

  // 边缘模式
  //  1. 自然边界
  //  2. 固定边界
  //  3. 周期边界
  int side_mode;

  // 固定边界的导数
  float side_A, side_B;

  // 存储插值点
  std::vector<POINT> insert_points;

  // 一阶和二阶的差分数组
  std::vector<float> dy;
  std::vector<float> dx;
  std::vector<float> ddx;

  // 是否被修改
  bool is_modified();

  // 懒计算出的 $c_i$ 的数量
  // num_insertpoints() - c_i.size() = k
  // 对于任意 $i >= k$，$c_i$ 均已被计算

  // 缓存 beta_i
  std::vector<float> beta_i;

  // 缓存 c_i
  std::vector<float> c_i;

  // 缓存 y_i
  std::vector<float> y_i;

  // 计算 c_i
  void calculate_c(int k_this, float * ans);

public:

  // 初始化函数
  CSpline(int side_mode = 1);

  // 设置插值模式
  void set_side_mode(int c_side_mode);

  // 设置固定边界的导数
  void set_side_der(float A, float B);

  // 插入新的插值点
  // 之间进行差分数组和 beta 的计算
  void push_back(POINT input_point);

  // 获取一个节点
  // 懒计算的核心函数
  float get_value(float x);

  // 获取插值点的数量
  // 从num_points.size() 获取
  int num_insertpoints();

  //test
  void test();
};

#endif //__CSPLINE_H__

