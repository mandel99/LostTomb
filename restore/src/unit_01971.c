#include "restore_types.h"
#define DAT_00799af0 (*(undefined4 *)0x00799af0u)

undefined1 FUN_004e5790(int *param_1,int param_2,int param_3)

{
  bool bVar1;
  undefined1 uVar2;
  int iVar3;
  int iVar4;
  short *psVar5;
  
  bVar1 = false;
  uVar2 = 0;
  iVar4 = 1;
  psVar5 = &DAT_00799af0;
  do {
    if (param_3 <= iVar4) {
      return uVar2;
    }
    if (((char)psVar5[-8] == '\x01') && (*psVar5 == param_2)) {
      bVar1 = true;
      iVar3 = (**(code **)(*param_1 + 0xc))(iVar4,0);
      if (iVar3 < 100) {
        uVar2 = 1;
      }
    }
    iVar4 = iVar4 + 1;
    psVar5 = psVar5 + 0x84;
  } while (!bVar1);
  return uVar2;
}
