#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_00799a64 (*(undefined4 *)0x00799a64u)

int FUN_0046f1c0(int param_1)

{
  int iVar1;
  
  iVar1 = 0;
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    iVar1 = (*(int *)(&DAT_00799a64 + param_1 * 0x108) + 5000) / 10000;
  }
  return iVar1;
}
