#include "restore_types.h"
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
extern void FUN_004cd510();

void FUN_004cd890(int param_1,int param_2,int param_3,int param_4)

{
  if (param_1 < 1) {
    param_1 = 0;
  }
  else if (DAT_00e38d9c <= param_1 + param_3) {
    param_3 = DAT_00e38d9c - param_1;
  }
  if (param_2 < 1) {
    param_2 = 0;
  }
  else if (DAT_00e38d98 <= param_2 + param_4) {
    param_4 = DAT_00e38d98 - param_2;
  }
  param_3 = param_1 + -1 + param_3;
  FUN_004cd510(param_1,param_2,param_3,param_2,0x8410);
  param_4 = param_2 + -1 + param_4;
  FUN_004cd510(param_3,param_2,param_3,param_4,0xffff);
  FUN_004cd510(param_1,param_4,param_3,param_4,0xffff);
  FUN_004cd510(param_1,param_2,param_1,param_4,0x8410);
  return;
}
