
/*
* 这个类用于实现 LBM 方法的计算
*/

/*
 * 尝试添加权重的更新来实现拓扑优化
 */

#ifndef __LBM_H__
#define __LBM_H__

#include "PLANE.h"
#include "FUNCTION.h"
#pragma GCC optimize(3,"Ofast","inline")
class LBM{
  /*
  * 他应当存储这些变量
  *   分布函数，f 和 F
  *   计算域 PLANE 的索引
  * 他应当拥有下面这些常量
  *   速度方向，D2Q9，提供两种数据类型
  *   速度方向对应的权重
  *   Re, niu, rt
  */
  float *f;
  float *F;

  PLANE & pl;

  POINT e[9];
  IDX E[9];

  float **w;
  float w_base[9];
  float Re, niu, rt;

  int NX, NY; // 用于更方便的获取计算域大小
  
  float alpha, beta; // 用于更新权重 alpha -> / ; beta -> \
  
  FUNCTION &Fun; //激活函数

  public:

  /*
  * 他应当拥有如下方法
  *   用于在分布函数中转换坐标的索引函数
  *   构造函数
  *   初始化函数
  *   计算平衡态分布函数
  *   更新分布函数
  *   更新速度函数
  *   边界处理函数
  *
  *   计算一次
  *   计算多次
  *
  *   存储结果
  */

  int idx(int dir, IDX pos);
  LBM(PLANE & pl_, FUNCTION & Fun_);
  // 这里用于更新计算域
  void NX_update();
  void NY_update();
  void N_update();
  void INIT();
  float feq(int dir, float rho, VECTOR u, IDX pos);
  void FLUID_FIELD();
  void MACRO();
  void FLUID_BOUNDARY();
  
  void RUN_ONE_STEP();

  void WEIGHT_OPT(); // 更新权重
  void RUN_MULTI_STEP(int T);
  
  void RUN_ONE_STEP_WITH_OPT();
  void RUN_MULTI_STEP_WITH_OPT(int T);

  void SAVE();
};

#endif //__LBM_H__
