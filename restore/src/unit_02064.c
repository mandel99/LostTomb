#include "restore_types.h"
#define DAT_00784e78 (*(undefined4 *)0x00784e78u)

uint FUN_004ea930(void)

{
  int iVar1;
  uint uVar2;
  uint uVar3;
  int *piVar4;
  
  piVar4 = &DAT_00784e78;
  uVar2 = 0xffffffff;
  do {
    switch((int)(uVar2)) {
    case 0: ;
    case 7: ;
    case 9: ;
    case 0xc: ;
    case 0xe: ;
    case 0x10: ;
    case 0x11: ;
    case 0x12: ;
    case 0x13: ;
    case 0x16: ;
    case 0x17: ;
    case 0x18: ;
    case 0x19: ;
    case 0x1b: ;
    case 0x1d: ;
      if (*piVar4 < piVar4[-0x24]) {
        return uVar2 & 0xffffff00;
      }
    }
    uVar3 = uVar2 + 1;
    piVar4 = piVar4 + 1;
    iVar1 = uVar2 + 2;
    uVar2 = uVar3;
  } while (iVar1 < 0x24);
  return CONCAT31((int3)(uVar3 >> 8),1);
}
