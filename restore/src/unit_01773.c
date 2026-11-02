#include "restore_types.h"
#define DAT_00e38e68 (*(uint32_t *)0x00e38e68u)
#define DAT_00e390fc (*(uint32_t *)0x00e390fcu)
#define DAT_00e39104 (*(uint32_t *)0x00e39104u)
#define _DAT_00e390f4 (*(undefined4 *)0x00e390f4u)
extern void FUN_004d0280();
extern void FUN_0051f780();

void FUN_004cf8e0(void)

{
  DWORD DVar1;
  DWORD DVar2;
  int iVar3;
  undefined4 uVar4;
  
  DVar1 = timeGetTime();
  iVar3 = 3;
  do {
    if (DAT_00e38e68 == 1) {
      uVar4 = 0x14;
LAB_004cf910: ;
      FUN_0051f780(uVar4);
    }
    else {
      if (DAT_00e38e68 == 2) {
        uVar4 = 0x1e;
        goto LAB_004cf910;
      }
      if (DAT_00e38e68 == 3) {
        uVar4 = 0x28;
        goto LAB_004cf910;
      }
    }
    iVar3 = iVar3 + -1;
    if (iVar3 == 0) {
      DVar2 = timeGetTime();
      DAT_00e390fc = DVar2 - DVar1;
      DVar1 = timeGetTime();
      iVar3 = 3;
      do {
        FUN_004d0280(1);
        iVar3 = iVar3 + -1;
      } while (iVar3 != 0);
      DVar2 = timeGetTime();
      _DAT_00e390f4 = DVar2 - DVar1;
      DAT_00e39104 = (DVar2 - DVar1) + DAT_00e390fc;
      return;
    }
  } while( true );
}
