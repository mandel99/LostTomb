#include "restore_types.h"
#define DAT_005c274c (*(undefined4 *)0x005c274cu)

void FUN_004204d0(int param_1,int *param_2)

{
  int *piVar1;
  
  piVar1 = &DAT_005c274c;
  *param_2 = 0;
  do {
    if ((*(short *)(piVar1[1] + 0x12) != 0) && (*piVar1 == param_1)) {
      *param_2 = *param_2 + 1;
    }
    piVar1 = piVar1 + 4;
  } while ((int)piVar1 < 0x5c28ac);
  return;
}
