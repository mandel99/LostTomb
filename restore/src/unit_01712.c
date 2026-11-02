#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
extern void FUN_004cc770();

void FUN_004cd390(undefined4 param_1,short *param_2,undefined4 param_3,undefined4 param_4,
                 int param_5,int param_6)

{
  FUN_004cc770(param_1,*param_2 + DAT_00e38da8 + param_5,param_2[1] + DAT_00e38da4 + param_6,param_3
               ,param_4);
  return;
}
