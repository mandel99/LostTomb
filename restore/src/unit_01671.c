#include "restore_types.h"

int FUN_004cc0e0(byte *param_1)

{
  byte bVar1;
  int iVar2;
  int iVar3;
  
  iVar2 = 0;
  iVar3 = 1;
  do {
    bVar1 = *param_1;
    if ((0x2f < bVar1) && (bVar1 < 0x3a)) {
      return iVar2;
    }
    if (bVar1 == 0x2d) {
      return iVar2;
    }
    param_1 = param_1 + 1;
    iVar2 = iVar2 + 1;
    iVar3 = iVar3 + 1;
  } while (iVar3 < 1000);
  return iVar2;
}
