#include "restore_types.h"
#define DAT_005c274c (*(undefined4 *)0x005c274cu)
#define DAT_00ea5ba8 (*(undefined4 *)0x00ea5ba8u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)

void FUN_00420600(int *param_1,int *param_2,int *param_3,int *param_4,int *param_5,int *param_6)

{
  int *piVar1;
  
  *param_5 = 0;
  *param_3 = 0;
  *param_6 = 0;
  *param_4 = 0;
  piVar1 = &DAT_005c274c;
  do {
    if (*piVar1 == 0x4f) {
      *param_3 = *param_3 + 1;
    }
    else {
      *param_5 = *param_5 + 1;
    }
    if (piVar1[-1] < (int)*(short *)((int)&DAT_00ea5ba8 + DAT_00eaac28 * 0x49d8 + 2)) {
      if (*piVar1 == 0x4f) {
        *param_4 = *param_4 + 1;
      }
      else {
        *param_6 = *param_6 + 1;
      }
    }
    piVar1 = piVar1 + 4;
  } while ((int)piVar1 < 0x5c28ac);
  *param_1 = (*param_4 * 100) / *param_3;
  *param_2 = (*param_6 * 100) / *param_5;
  return;
}
