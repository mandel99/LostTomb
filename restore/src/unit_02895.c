#include "restore_types.h"
extern uint FUN_00564ab1();

void FUN_00565307(uint param_1,int *param_2,int *param_3)

{
  int *piVar1;
  
  piVar1 = param_2 + 1;
  *piVar1 = *piVar1 + -1;
  if (*piVar1 < 0) {
    param_1 = FUN_00564ab1(param_1,param_2);
  }
  else {
    *(undefined1 *)*param_2 = (undefined1)param_1;
    *param_2 = *param_2 + 1;
    param_1 = param_1 & 0xff;
  }
  if (param_1 == 0xffffffff) {
    *param_3 = -1;
    return;
  }
  *param_3 = *param_3 + 1;
  return;
}
