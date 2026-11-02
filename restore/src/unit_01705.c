#include "restore_types.h"
#define DAT_00e20760 (*(int32_t *)0x00e20760u)
extern int FUN_004cc020();

int FUN_004cd120(byte *param_1,undefined4 param_2)

{
  byte bVar1;
  bool bVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  
  bVar2 = false;
  DAT_00e20760 = 0;
  iVar4 = 0;
  iVar5 = 1;
  do {
    bVar1 = *param_1;
    param_1 = param_1 + 1;
    if (bVar1 == 0) {
      return iVar4;
    }
    if (bVar1 == 0x20) {
      if (bVar2) {
        return iVar4;
      }
      iVar4 = iVar4 + 4;
    }
    else if (bVar1 == 0x24) {
      if (bVar2) {
        return iVar4;
      }
    }
    else if (0x1f < bVar1) {
      iVar3 = FUN_004cc020(bVar1,param_2);
      iVar4 = iVar4 + iVar3;
      bVar2 = true;
    }
    DAT_00e20760 = DAT_00e20760 + 1;
    iVar5 = iVar5 + 1;
  } while (iVar5 < 2000);
  return iVar4;
}
