#include "restore_types.h"
#define DAT_00705650 (*(undefined4 *)0x00705650u)
#define DAT_0077591c (*(undefined *)0x0077591cu)
#define DAT_00e38dca (*(byte *)0x00e38dcau)
#define DAT_00e38dda (*(char *)0x00e38ddau)
extern undefined4 FUN_00443000();
extern undefined4 FUN_00443040();
extern void FUN_00444a80();

void FUN_00408540(void)

{
  undefined4 uVar1;
  int iVar2;
  int iVar3;
  int iVar4;
  int *piVar5;
  undefined4 *puVar6;
  
  DAT_00e38dda = 0x24;
  FUN_00444a80();
  uVar1 = DAT_0077591c;
  iVar3 = 0;
  iVar4 = 1;
  piVar5 = &DAT_00705650;
  do {
    iVar2 = FUN_00443000(uVar1,iVar4);
    if (iVar2 == 0) {
      iVar2 = FUN_00443040(uVar1,iVar4);
      if (iVar2 != 0) goto LAB_0040857e;
    }
    else {
LAB_0040857e: ;
      *piVar5 = iVar4;
      iVar3 = iVar3 + 1;
      piVar5 = piVar5 + 1;
    }
    iVar4 = iVar4 + 1;
    if (0x23 < iVar4) {
      if (iVar3 < 0x10) {
        puVar6 = &DAT_00705650 + iVar3;
        for (iVar4 = 0x10 - iVar3; iVar4 != 0; iVar4 = iVar4 + -1) {
          *puVar6 = 0xffffffff;
          puVar6 = puVar6 + 1;
        }
      }
      DAT_00e38dca = 1;
      return;
    }
  } while( true );
}
