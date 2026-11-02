#include "restore_types.h"
#define DAT_007999ea (*(undefined4 *)0x007999eau)
extern void FUN_00421120();

void FUN_004e6e70(int param_1,undefined4 param_2,undefined4 param_3)

{
  int in_ECX;
  
  if (*(int *)(in_ECX + 0x58 + (short)(&DAT_007999ea)[param_1 * 0x84] * 0x34) == 0) {
    FUN_00421120(param_1,param_2,param_3);
  }
  return;
}
