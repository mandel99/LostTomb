#include "restore_types.h"
#define DAT_005794bc (*(undefined4 *)0x005794bcu)
#define DAT_006fdc88 (*(undefined *)0x006fdc88u)
#define DAT_006fdc96 (*(undefined1 *)0x006fdc96u)
#define DAT_007027b8 (*(char *)0x007027b8u)
#define DAT_00e38ea8 (*(undefined1 *)0x00e38ea8u)
#define DAT_00e38ebc (*(undefined *)0x00e38ebcu)
#define DAT_01010101 (*(undefined4 *)0x01010101u)
#define _DAT_006fdc88 (*(undefined4 *)0x006fdc88u)
#define _DAT_006fdc8c (*(undefined4 *)0x006fdc8cu)
#define _DAT_006fdc90 (*(undefined4 *)0x006fdc90u)
#define _DAT_006fdc94 (*(undefined4 *)0x006fdc94u)

void FUN_004179f0(void)

{
  char cVar1;
  int iVar2;
  int *piVar3;
  int *piVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  
  if ((DAT_00e38ea8 < '\v') && (DAT_00e38ebc == 0)) {
    iVar7 = 0;
    piVar4 = &DAT_005794bc;
    iVar5 = (int)DAT_00e38ea8;
    do {
      if ((iVar5 == piVar4[-3]) && (iVar6 = *piVar4, 0 < iVar6)) {
        cVar1 = (&DAT_007027b8)[iVar7];
        piVar3 = (int *)piVar4[1];
        do {
          iVar2 = *piVar3;
          piVar3 = piVar3 + 1;
          iVar6 = iVar6 + -1;
          (&DAT_006fdc88)[iVar2] = cVar1 != '\0';
        } while (iVar6 != 0);
      }
      piVar4 = piVar4 + 7;
      iVar7 = iVar7 + 1;
    } while ((int)piVar4 < 0x579794);
    return;
  }
  _DAT_006fdc88 = &DAT_01010101;
  _DAT_006fdc8c = &DAT_01010101;
  _DAT_006fdc90 = &DAT_01010101;
  _DAT_006fdc94 = 0x101;
  DAT_006fdc96 = 1;
  return;
}
