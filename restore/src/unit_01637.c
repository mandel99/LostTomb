#include "restore_types.h"
#define DAT_00576104 (*(undefined4 *)0x00576104u)
#define DAT_00e38da4 (*(undefined *)0x00e38da4u)
#define DAT_00e38da8 (*(undefined *)0x00e38da8u)
extern undefined4 FUN_004cf620();

int FUN_004c92a0(void)

{
  int iVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  
  piVar5 = &DAT_00576104;
  do {
    iVar4 = piVar5[-0x11];
    iVar1 = *piVar5;
    iVar2 = iVar4 * 0x23;
    iVar3 = FUN_004cf620(iVar2 + 0x15 + DAT_00e38da8,DAT_00e38da4 + 0x32,0x18,0x18);
    if (iVar3 != 0) {
      return iVar4;
    }
    if (((iVar1 != 0x1b) && (-1 < iVar1)) && (iVar1 < 0x24)) {
      iVar4 = FUN_004cf620(iVar2 + 0x15 + DAT_00e38da8,DAT_00e38da4 + 0x86,0x18,0x18);
      if (iVar4 != 0) {
        return iVar1;
      }
    }
    piVar5 = piVar5 + 1;
  } while ((int)piVar5 < 0x576145);
  return 0;
}
