#include "restore_types.h"
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
extern void FUN_004cce00();

void FUN_004cd290(undefined4 param_1,undefined4 param_2,undefined4 param_3,short *param_4,
                 undefined4 param_5,undefined4 param_6,int param_7,int param_8)

{
  FUN_004cce00(param_1,param_2,param_3,*param_4 + DAT_00e38da8 + param_7,
               param_4[1] + DAT_00e38da4 + param_8,param_5,param_6);
  return;
}
