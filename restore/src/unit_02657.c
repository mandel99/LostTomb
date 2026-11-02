#include "restore_types.h"

void FUN_0055d110(int param_1,int param_2,int param_3,int param_4)

{
  byte bVar1;
  uint uVar2;
  
  while (param_1 = param_1 + -1, -1 < param_1) {
    bVar1 = *(byte *)(param_2 + param_1);
    uVar2 = (uint)*(byte *)(param_3 + param_1);
    do {
      *(char *)(param_4 + uVar2) = (char)param_1;
      uVar2 = uVar2 + (1 << (bVar1 & 0x1f));
    } while (uVar2 < 0x100);
  }
  return;
}
