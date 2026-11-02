#include "restore_types.h"

void FUN_004cbf10(undefined1 *param_1,undefined1 *param_2)

{
  param_2[1] = 0;
  for (; param_1 < param_2; param_2 = param_2 + -1) {
    *param_2 = param_2[-1];
  }
  return;
}
