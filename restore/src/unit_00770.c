#include "restore_types.h"
#define DAT_00785d70 (*(undefined *)0x00785d70u)
#define DAT_00785d9c (*(undefined4 *)0x00785d9cu)

uint FUN_0044ef60(void)

{
  uint uVar1;
  uint uVar2;
  int in_ECX;
  
  uVar1 = (uint)*(short *)(in_ECX + 0x34);
  uVar2 = uVar1;
  if ((-1 < (int)uVar1) && ((int)uVar1 < DAT_00785d70)) {
    uVar2 = 0;
    if (uVar1 * 0x7c != -0x785d74) {
      return (uint)(&DAT_00785d9c)[uVar1 * 0x1f] >> 4 & 0xffffff01;
    }
  }
  return uVar2 & 0xffffff00;
}
