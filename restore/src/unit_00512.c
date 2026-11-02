#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999f0 (*(undefined4 *)0x007999f0u)
extern int FUN_004cde60();

int FUN_00434a10(int param_1)

{
  int iVar1;
  
  if (*(char *)((int)&DAT_007999dc + param_1 * 0x108 + 1) != '\0') {
    return 0;
  }
  iVar1 = FUN_004cde60((int)(short)(&DAT_007999f0)[param_1 * 0x84],300);
  return iVar1 / 10;
}
