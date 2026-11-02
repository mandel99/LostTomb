#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
extern void FUN_004361a0();
extern int FUN_004cde60();

void FUN_00436140(int param_1,undefined4 param_2,undefined4 param_3)

{
  undefined4 uVar1;
  
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) == '\0') {
    uVar1 = FUN_004cde60((int)(short)(&DAT_007999f0)[param_1 * 0x84],300);
    FUN_004361a0(uVar1,param_2,param_3,0);
  }
  return;
}
