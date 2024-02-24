
#include "FUNCTION.h"

FUNCTION::FUNCTION() { return; }

float FUNCTION::operator()(float x) {
  // return 2. / (1. + exp(-20. * (x - 0))) - 1.;
  // relu
  return x > 0. ? x : 0.;
}
