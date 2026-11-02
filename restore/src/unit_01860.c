#include "restore_types.h"
extern void * FUN_00561a1d();
extern void FUN_00561a2b();
extern uint FUN_00561ca1();

void FUN_004d6900(int param_1,int param_2,undefined4 *param_3)

{
  void *pvVar1;
  
  if (((*(byte *)(param_1 + 0xc) & 0x20) == 0) && (*param_3 = 0, 0 < param_2)) {
    do {
      pvVar1 = FUN_00561a1d(0x1c);
      FUN_00561ca1(pvVar1,4,1,param_1);
      FUN_00561ca1((int)pvVar1 + 4,4,1,param_1);
      FUN_00561ca1((int)pvVar1 + 8,4,1,param_1);
      FUN_00561ca1((int)pvVar1 + 0xc,4,1,param_1);
      FUN_00561ca1((int)pvVar1 + 0x10,4,1,param_1);
      FUN_00561ca1((int)pvVar1 + 0x14,4,1,param_1);
      *(undefined4 *)((int)pvVar1 + 0x18) = *param_3;
      if ((*(byte *)(param_1 + 0xc) & 0x30) == 0) {
        *param_3 = pvVar1;
      }
      else {
        FUN_00561a2b(pvVar1);
      }
      param_2 = param_2 + -1;
    } while (param_2 != 0);
  }
  return;
}
