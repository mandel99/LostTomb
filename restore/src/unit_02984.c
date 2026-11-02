#include "restore_types.h"
#define _DAT_0126640c (*(undefined4 *)0x0126640cu)
extern void * FUN_00561b68();

void FUN_0056917b(undefined4 *param_1)

{
  void *pvVar1;
  
  _DAT_0126640c = _DAT_0126640c + 1;
  pvVar1 = FUN_00561b68(0x1000);
  param_1[2] = pvVar1;
  if (pvVar1 == (void *)0x0) {
    param_1[3] = param_1[3] | 4;
    param_1[2] = param_1 + 5;
    param_1[6] = 2;
  }
  else {
    param_1[3] = param_1[3] | 8;
    param_1[6] = 0x1000;
  }
  param_1[1] = 0;
  *param_1 = param_1[2];
  return;
}
