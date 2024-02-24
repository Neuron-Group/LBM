
#include "Cspline.h"
#include "POINT.h"
#include <algorithm>

CSpline::CSpline(int side_mode) {
  num_points = 0;
  this->side_mode = side_mode;
  side_A = side_B = 0;
  insert_points.clear();
  dy.clear();
  dx.clear();
  ddx.clear();
  is_modified();
  beta_i.clear();
  c_i.clear();
  y_i.clear();
}

bool CSpline::is_modified() {
  if (num_points == insert_points.size()) {
    return false;
  }
  return true;
}

void CSpline::set_side_mode(int c_side_mode) { this->side_mode = c_side_mode; }

void CSpline::set_side_der(float A, float B) {
  this->side_A = A;
  this->side_B = B;
}

int CSpline::num_insertpoints() { return insert_points.size(); }

void CSpline::calculate_c(int k_this, float *ans) {

  if (k_this >= 10) {
    k_this = 9;
  }

  if (k_this < 0) {
    k_this = 0;
  }

  // ans[0] = $c_i$;
  // ans[1] = $c_{i+1}$;
  //
  // shall: i+1 <= n-1;

  // k_this 正向下标
  // 用于带入方程计算的下标
  int i_this = this->num_insertpoints() - k_this - 1;
  // i_this 反向下标
  // 真正c_i序列的下标

  // 这个函数用于补全 c_i 直到 c_i 序列的长度加上正向下标大于等于 n
  int i_this_c, k_this_c;
  while (c_i.size() + k_this < num_insertpoints()) {
    // c_i.size(): c_i 的大小，它减去1是列表中最大的反向下标
    // i_this_c = 目前需要计算的反向下标。
    i_this_c = c_i.size();

    // k_this_c = 目前需要计算的正向下标
    // 将这个下标带入方程进行计算
    k_this_c = this->num_insertpoints() - i_this_c - 1;

    // std::cout << i_this_c << " " << k_this_c << std::endl;

    if (k_this_c == num_insertpoints() - 1) {
      c_i.push_back(y_i[k_this_c] / beta_i[k_this_c]);
    } else {
      float cci;
      if (side_mode == 1 && k_this_c == 0) {
        cci = 0.;
      } else {
        cci = dx[k_this_c];
      }
      c_i.push_back((y_i[k_this_c] - cci * c_i[i_this_c - 1]) /
                    beta_i[k_this_c]);
      // std::cout << y_i[k_this_c] << " " << beta_i[k_this_c] << " "
      //           << c_i[i_this_c - 1] << std::endl;
    }
  }

  ans[0] = c_i[i_this];
  ans[1] = c_i[i_this - 1];
}

// 插入计算：
// 在节点列表中插入节点
// 更新所有的差分数组
// 重新计算 $beta_(n-1)$
// 计算 $beta_(n)$
// 重新计算 $y_(n-1)$
// 计算 $y_n$
void CSpline::push_back(POINT input_point) {
  insert_points.push_back(input_point);
  if (num_insertpoints() > 1) {
    dx.push_back(insert_points[insert_points.size() - 1].x() -
                 insert_points[insert_points.size() - 2].x());

    dy.push_back(insert_points[insert_points.size() - 1].y() -
                 insert_points[insert_points.size() - 2].y());
  }

  if (num_insertpoints() > 2) {
    ddx.push_back(dx[dx.size() - 1] - dx[dx.size() - 2]);
  }

  int nump = num_insertpoints();

  if (nump == 1) {
    if (side_mode == 1) {
      beta_i.push_back(1.);
      y_i.push_back(0.);
    } else if (side_mode == 2) {
      beta_i.push_back(1.);
      y_i.push_back(0.);
    }
  } else if (nump == 2) {
    if (side_mode == 1) {
      beta_i.push_back(1.);
      y_i.push_back(0.);
    } else if (side_mode == 2) {
      beta_i[0] = 2. * dx[0];
      beta_i.push_back((3. / 2.) * dx[0]);
      y_i[0] = 3. * ((dy[0] / dx[0]) - side_A);
      y_i.push_back((3. * (side_B - (dy[0] / dx[0]))) -
                    (dx[0] * y_i[0] / beta_i[0]));
    }
  } else {
    float cci;
    if (nump == 3 && side_mode == 1) {
      cci = 0.;
    } else {
      cci = dx[nump - 3];
    }

    int i = nump - 2;

    beta_i[i] = 2 * (dx[i - 1] + dx[i]) - (dx[i - 1] * cci / beta_i[i - 1]);

    // int i_this = nump - 2;
    y_i[i] = 3 * (dy[i] / dx[i] - dy[i - 1] / dx[i - 1]) -
             (dx[i - 1] * y_i[i - 1] / beta_i[i - 1]);

    if (side_mode == 1) {
      beta_i.push_back(1.);
      y_i.push_back(0.);
    } else {
      // $ a_(i-1) = h_(i-2) $
      beta_i.push_back(2. * dx[nump - 2] -
                       (dx[nump - 2] * dx[nump - 2] / beta_i[nump - 2]));
      y_i.push_back(3. * (side_B - (dy[nump - 2] / dx[nump - 2])) -
                    dx[nump - 2] * y_i[nump - 2] / beta_i[nump - 2]);
    }
  }
}

// 取值操作：
// 如果已经被插入新的节点
//  更新 num_points
//  清空 c_i
// 二分算法判断包含给定 $x$ 的区间 $[x_i, x_(i+1)]$
// 计算 c_i
float CSpline::get_value(float x) {
  if (this->num_insertpoints() == 0) {
    return 0;
  }
  if (this->num_insertpoints() == 1) {
    if (side_mode == 1) {
      return insert_points[0].y();
    } else if (side_mode == 2) {
      return side_A * (x - insert_points[0].x()) + insert_points[0].y();
    } else {
      return 0;
    }
  }

  if (is_modified()) {
    c_i.clear();
    num_points = num_insertpoints();
  }
  POINT test_x(x, 0);
  int k_this = std::lower_bound(insert_points.begin(), insert_points.end(),
                                test_x, cmp_x) -
               insert_points.begin();

  k_this--;

  if (k_this >= num_insertpoints() - 1) {
    k_this = num_insertpoints() - 2;
  }

  if (k_this < 0) {
    k_this = 0;
  }

  float ans_c[2];
  calculate_c(k_this, ans_c);

  float ans_a_i = this->insert_points[k_this].y();
  float ans_b_i = dy[k_this] / dx[k_this] - ans_c[0] * dx[k_this] -
                  (dx[k_this] * (ans_c[1] - ans_c[0]) / 3.);
  float ans_c_i = ans_c[0];
  float ans_d_i = (1. / 3.) * ((ans_c[1] - ans_c[0]) / dx[k_this]);

  float X = x - this->insert_points[k_this].x();

  return ans_a_i + ans_b_i * X + ans_c_i * X * X + ans_d_i * X * X * X;
}

void CSpline::test() {
  if (this->num_insertpoints() == 0) {
    std::cout << "nothing here.\n" << std::endl;
    return;
  }
  // 打印三对角矩阵 A 和向量 d
  std::cout << "A = [" << std::endl;
  for (int i = 0; i < this->num_insertpoints(); i++) {
    for (int j = 0; j < 4; j++) {
      std::cout << " ";
    }
    for (int j = 0; j < this->num_insertpoints(); j++) {
      if (j == i - 1) {
        if (i == this->num_insertpoints() - 1) {
          if (side_mode == 1) {
            std::cout << 0;
          } else if (side_mode == 2) {
            std::cout << dx[this->num_insertpoints() - 2];
          } else {
            std::cout << 0;
          }
        } else {
          std::cout << dx[i - 1];
        }
      } else if (j == i) {
        if (i == 0) {
          if (side_mode == 1) {
            std::cout << 1;
          } else if (side_mode == 2) {
            if (this->num_insertpoints() <= 1) {
              std::cout << 1;
            } else {
              std::cout << 2. * dx[0];
            }
          } else {
            std::cout << 0;
          }
        } else if (i == num_insertpoints() - 1) {
          if (side_mode == 1) {
            std::cout << 1;
          } else if (side_mode == 2) {
            if (this->num_insertpoints() <= 1) {
              std::cout << 1;
            } else {
              std::cout << 2. * dx[num_insertpoints() - 2];
            }
          } else {
            std::cout << 0;
          }
        } else {
          std::cout << 2. * (dx[i - 1] + dx[i]);
        }
      } else if (j == i + 1) {
        if (i == 0) {
          if (side_mode == 1) {
            std::cout << 0;
          } else if (side_mode == 2) {
            if (this->num_insertpoints() <= 1) {
              std::cout << 1;
            } else {
              std::cout << dx[0];
            }
          } else {
            std::cout << 0;
          }
        } else {
          std::cout << dx[i];
        }
      } else {
        std::cout << 0;
      }
      if (j == num_insertpoints() - 1) {
        std::cout << ";";
      } else {
        std::cout << ", ";
      }
    }
    std::cout << std::endl;
  }

  std::cout << "];\n" << std::endl;

  std::cout << "d = [" << std::endl;

  for (int i = 0; i < 4; i++) {
    std::cout << " ";
  }

  for (int i = 0; i < num_insertpoints(); i++) {
    if (i == 0) {
      if (side_mode == 1) {
        std::cout << 0 << ", ";
      } else if (side_mode == 2) {
        if (this->num_insertpoints() <= 1) {
          std::cout << 1 << ", ";
        } else {
          std::cout << 3. * (dy[0] / dx[0] - side_A) << ", ";
        }
      } else {
        std::cout << 0 << ", ";
      }
    } else if (i == num_insertpoints() - 1) {
      if (side_mode == 1) {
        std::cout << 0 << ", ";
      } else if (side_mode == 2) {
        if (num_insertpoints() <= 1) {
          std::cout << 0 << ", ";
        } else {
          std::cout << 3 * (side_B - dy[num_insertpoints() - 2] /
                                         dx[num_insertpoints() - 2])
                    << ", ";
        }
      } else {
        std::cout << 0 << ", ";
      }
    } else {
      std::cout << 3 * (dy[i] / dx[i] - dy[i - 1] / dx[i - 1]) << ", ";
    }
  }

  std::cout << "\n]';\n" << std::endl;

  std::cout << "c = [" << std::endl;
  for (int i = 0; i < 4; i++) {
    std::cout << " ";
  }
  for (int i = c_i.size() - 1; i >= 0; i--) {
    std::cout << c_i[i] << ", ";
  }
  std::cout << "\n]';\n" << std::endl;
}
