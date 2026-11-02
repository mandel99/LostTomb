#include "restore_types.h"

void FUN_004cbea0(int param_1,undefined1 *param_2,int param_3)

{
  if (0 < param_3) {
    param_1 = param_1 - (int)param_2;
    do {
      *param_2 = param_2[param_1];
      param_2 = param_2 + 1;
      param_3 = param_3 + -1;
    } while (param_3 != 0);
  }
  return;
}
