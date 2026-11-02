#include "restore_types.h"
#define DAT_00bcd786 (*(undefined4 *)0x00bcd786u)

int FUN_00462140(int param_1,uint param_2)

{
  byte *pbVar1;
  int iVar2;
  
  iVar2 = 0;
  pbVar1 = &DAT_00bcd786;
  do {
    if (((pbVar1[4] == 1) && (*pbVar1 == param_2)) && (*(short *)(pbVar1 + 0x50) == param_1)) {
      iVar2 = iVar2 + 1;
    }
    pbVar1 = pbVar1 + 0x184;
  } while ((int)pbVar1 < 0xc8ad42);
  return iVar2;
}
