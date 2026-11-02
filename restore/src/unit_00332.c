#include "restore_types.h"
#define DAT_00596084 (*(undefined4 *)0x00596084u)

void FUN_004204a0(int param_1,int *param_2)

{
  int *piVar1;
  
  piVar1 = &DAT_00596084;
  *param_2 = 0;
  do {
    if ((*(short *)(piVar1[1] + 0x12) != 0) && (*piVar1 == param_1)) {
      *param_2 = *param_2 + 1;
    }
    piVar1 = piVar1 + 4;
  } while ((int)piVar1 < 0x596154);
  return;
}
