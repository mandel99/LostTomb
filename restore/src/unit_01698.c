#include "restore_types.h"
#define DAT_0057c048 (*(undefined4 *)0x0057c048u)
#define DAT_00e20778 (*(int32_t *)0x00e20778u)
extern void FUN_004ccbb0();
extern void FUN_004cce00();

void FUN_004cccb0(int param_1,int param_2,int param_3,undefined4 param_4,undefined4 param_5,
                 undefined4 param_6)

{
  DAT_00e20778 = 0;
  if (param_2 < 0) {
    FUN_004cce00((int32_t)(0u-(uint32_t)param_2),0x20,&DAT_0057c048,param_3,param_4,param_5,param_6);
    if (param_1 == 1) {
      DAT_00e20778 = DAT_00e20778 + -8;
    }
    else if (param_1 == 2) {
      DAT_00e20778 = DAT_00e20778 + -0xb;
    }
    FUN_004ccbb0(0x14,0,DAT_00e20778 + param_3,param_4,param_5,param_6);
    return;
  }
  FUN_004ccbb0(0x14,1,param_3,param_4,param_5,param_6);
  FUN_004cce00(param_2,0x20,&DAT_0057c048,DAT_00e20778 + param_3,param_4,param_5,param_6);
  return;
}
