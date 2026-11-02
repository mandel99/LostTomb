#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
extern void FUN_004ccbb0();

void FUN_004cd1f0(undefined4 param_1,undefined4 param_2,short *param_3,undefined4 param_4,
                 undefined4 param_5,int param_6,int param_7)

{
  FUN_004ccbb0(param_1,param_2,*param_3 + DAT_00e38da8 + param_6,param_3[1] + DAT_00e38da4 + param_7
               ,param_4,param_5);
  return;
}
