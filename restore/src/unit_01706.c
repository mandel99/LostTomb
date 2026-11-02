#include "restore_types.h"
#define DAT_00e38da4 (*(int32_t *)0x00e38da4u)
#define DAT_00e38da8 (*(int32_t *)0x00e38da8u)
extern void FUN_00536c30();

void FUN_004cd1a0(undefined4 param_1,short *param_2,undefined4 param_3,int param_4,int param_5)

{
  FUN_00536c30(param_1,*param_2 + DAT_00e38da8 + param_4,param_2[1] + DAT_00e38da4 + param_5,
               (int)param_2[2] - (int)*param_2,(int)param_2[3] - (int)param_2[1],param_3);
  return;
}
