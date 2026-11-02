#include "restore_types.h"
#define DAT_00e38d98 (*(undefined *)0x00e38d98u)
#define DAT_00e38d9c (*(undefined *)0x00e38d9cu)
extern void FUN_004cd510();

void FUN_004cd9d0(int param_1,int param_2,int param_3,int param_4,undefined4 param_5)

{
  if ((0 < param_3) && (0 < param_4)) {
    if (param_1 < 1) {
      param_1 = 0;
    }
    if (DAT_00e38d9c <= param_1 + param_3) {
      param_3 = DAT_00e38d9c - param_1;
    }
    if (param_2 < 1) {
      param_2 = 0;
    }
    if (DAT_00e38d98 <= param_2 + param_4) {
      param_4 = DAT_00e38d98 - param_2;
    }
    param_4 = param_2 + param_4;
    if (param_2 < param_4) {
      do {
        FUN_004cd510(param_1,param_2,param_1 + -1 + param_3,param_2,param_5);
        param_2 = param_2 + 1;
      } while (param_2 < param_4);
    }
  }
  return;
}
