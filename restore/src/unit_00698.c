#include "restore_types.h"
#define DAT_006057f4 (*(undefined4 *)0x006057f4u)
extern undefined4 FUN_004493d0();
extern undefined4 FUN_004493e0();
extern undefined4 FUN_00449550();

int FUN_004493f0(void)

{
  char cVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  
  iVar4 = 0;
  iVar2 = FUN_004493d0();
  iVar3 = FUN_004493e0();
  if (iVar2 < iVar3) {
    piVar5 = &DAT_006057f4;
    do {
      cVar1 = FUN_00449550(iVar2);
      if (cVar1 != '\0') {
        *piVar5 = iVar2;
        iVar4 = iVar4 + 1;
        piVar5 = piVar5 + 1;
      }
      iVar2 = iVar2 + 1;
      iVar3 = FUN_004493e0();
    } while (iVar2 < iVar3);
    return iVar4;
  }
  return 0;
}
