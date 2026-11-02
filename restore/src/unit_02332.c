#include "restore_types.h"

undefined4 FUN_0051cba0(int param_1,int param_2,int param_3,int param_4)

{
  if (param_3 < param_1) {
    if (param_4 < param_2) {
      return 7;
    }
    if (param_2 == param_4) {
      return 6;
    }
    if (param_2 < param_4) {
      return 5;
    }
  }
  else if (param_1 == param_3) {
    if (param_4 < param_2) {
      return 0;
    }
    if (param_2 < param_4) {
      return 4;
    }
  }
  else if (param_1 < param_3) {
    if (param_4 < param_2) {
      return 1;
    }
    if (param_2 == param_4) {
      return 2;
    }
    if (param_2 < param_4) {
      return 3;
    }
  }
  return 8;
}
