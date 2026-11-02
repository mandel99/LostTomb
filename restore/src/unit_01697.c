#include "restore_types.h"
#define DAT_007027d4 (*(undefined4 *)0x007027d4u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
extern void FUN_004ccbb0();
extern void FUN_004cce00();

void FUN_004ccc30(undefined4 param_1,int param_2,int param_3,int param_4,undefined4 param_5,
                 undefined4 param_6,undefined4 param_7)

{
  undefined4 uVar1;
  int local_4;
  
  local_4 = 1;
  if ((param_3 == 1) || (param_3 == -1)) {
    local_4 = 0;
  }
  DAT_00e20778 = 0;
  if (param_3 < 0) {
    uVar1 = 0x2d;
    param_3 = (int32_t)(0u-(uint32_t)param_3);
  }
  else {
    uVar1 = 0x20;
  }
  FUN_004cce00(param_3,uVar1,&DAT_007027d4,param_4,param_5,param_6,param_7);
  FUN_004ccbb0(param_1,local_4 + param_2,DAT_00e20778 + param_4,param_5,param_6,param_7);
  return;
}
