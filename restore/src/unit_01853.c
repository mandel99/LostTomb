#include "restore_types.h"
extern void FUN_004d6250();

void FUN_004d6380(undefined4 param_1,undefined4 param_2,short *param_3,undefined4 param_4,
                 undefined4 param_5,int param_6,int param_7,int param_8,int param_9)

{
  FUN_004d6250(param_1,param_2,*param_3 + param_6 + param_8,param_3[1] + param_7 + param_9,
               (int)param_3[2] - (int)*param_3,param_4,param_5,param_8,param_9);
  return;
}
