#include "restore_types.h"
#define DAT_00978d9c (*(undefined4 *)0x00978d9cu)

void FUN_0047e250(char param_1,int param_2)

{
  byte bVar1;
  uint uVar2;
  
  bVar1 = param_1 * '\x03';
  uVar2 = (uint)(&DAT_00978d9c)[param_2] >> (bVar1 & 0x1f) & 7;
  if (uVar2 < 7) {
    (&DAT_00978d9c)[param_2] =
         (7 << (bVar1 & 0x1f) ^ 0xffffU) & (&DAT_00978d9c)[param_2] | uVar2 + 1 << (bVar1 & 0x1f);
  }
  return;
}
