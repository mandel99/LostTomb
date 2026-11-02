#include "restore_types.h"

undefined4 FUN_0045b490(int param_1)

{
  if (param_1 == 400) {
    return 3;
  }
  if (param_1 < 400) {
    if (param_1 > 299) {
      return 2;
    }
  }
  if (param_1 > 299) {
    return 0;
  }
  if (param_1 < 200) {
    return 0;
  }
  return 1;
}
