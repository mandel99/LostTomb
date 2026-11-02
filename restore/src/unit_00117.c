#include "restore_types.h"

void FUN_0040f820(undefined4 *param_1,undefined4 *param_2,int param_3)

{
  for (; 7 < param_3; param_3 = param_3 + -8) {
    *param_2 = *param_1;
    param_2[1] = param_1[1];
    param_1 = param_1 + 2;
    param_2 = param_2 + 2;
  }
  for (; 0 < param_3; param_3 = param_3 + -1) {
    *(undefined1 *)param_2 = *(undefined1 *)param_1;
    param_1 = (undefined4 *)((int)param_1 + 1);
    param_2 = (undefined4 *)((int)param_2 + 1);
  }
  return;
}
