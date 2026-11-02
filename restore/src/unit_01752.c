#include "restore_types.h"
#define DAT_00e36f10 (*(undefined *)0x00e36f10u)
extern undefined1 * FUN_0056256e();
extern int * FUN_00562644();
extern void FUN_005627a4();

void FUN_004ce8e0(void)

{
  undefined1 uVar1;
  undefined4 uVar2;
  undefined1 *puVar3;
  int iVar4;
  undefined1 local_4 [4];
  
  FUN_005627a4(local_4);
  uVar2 = FUN_00562644(local_4);
  puVar3 = (undefined1 *)FUN_0056256e(uVar2);
  iVar4 = 0;
  do {
    uVar1 = *puVar3;
    puVar3 = puVar3 + 1;
    *(undefined1 *)((int)&DAT_00e36f10 + iVar4) = uVar1;
    iVar4 = iVar4 + 1;
  } while (iVar4 < 0x1a);
  return;
}
