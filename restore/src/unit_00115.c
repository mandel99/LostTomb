#include "restore_types.h"
#define DAT_00605a6c (*(undefined4 *)0x00605a6cu)
#define DAT_00605b38 (*(undefined4 *)0x00605b38u)
#define DAT_00e92e0c (*(undefined *)0x00e92e0cu)

void FUN_0040f270(void)

{
  int iVar1;
  int *piVar2;
  int iVar3;
  int iVar4;
  undefined4 *puVar5;
  
  iVar1 = DAT_00e92e0c;
  puVar5 = (undefined4 *)&DAT_00605b38;
  for (iVar3 = 0xc; iVar3 != 0; iVar3 = iVar3 + -1) {
    *puVar5 = 0;
    puVar5 = puVar5 + 1;
  }
  *(undefined2 *)puVar5 = 0;
  *(undefined1 *)((int)puVar5 + 2) = 0;
  iVar3 = 0;
  iVar4 = 0;
  piVar2 = &DAT_00605a6c;
  do {
    if (*piVar2 + iVar4 <= iVar1) {
      (&DAT_00605b38)[iVar3] = 1;
      *piVar2 = iVar1;
    }
    piVar2 = piVar2 + 1;
    iVar3 = iVar3 + 1;
    iVar4 = iVar4 + 0x14;
  } while ((int)piVar2 < 0x605b38);
  return;
}
