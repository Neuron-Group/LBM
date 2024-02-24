
// #include "FUNCTION.h"
#include "LBM.h"
#pragma GCC optimize(3, "Ofast", "inline")
using namespace std;

signed main() {
  PLANE pl_test;
  LBM lbm(pl_test);
  lbm.RUN_MULTI_STEP(500000);
  // lbm.RUN_MULTI_STEP_WITH_OPT(5000);
  lbm.SAVE();
  return 0;
}
