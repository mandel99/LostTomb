#include "restore_types.h"
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaac30 (*(undefined4 *)0x00eaac30u)
#define DAT_00eaaf64 (*(undefined *)0x00eaaf64u)
extern bool FUN_004ecea0();
extern void FUN_004ed360();

void FUN_004f6130(void)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  int *piVar4;
  int iVar5;
  
  iVar5 = 0;
  if (0 < DAT_00eaaf64) {
    piVar4 = &DAT_00eaac30;
    do {
      iVar3 = *piVar4;
      bVar1 = *(byte *)(&DAT_00799a2c + iVar3 * 0x84);
      cVar2 = FUN_004ecea0((uint)bVar1);
      if (cVar2 != '\0') {
        iVar3 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) + *(int *)(&DAT_007999e4 + iVar3 * 0x108)
                ) - DAT_00bca2d8;
        FUN_004ed360(iVar3 % 0xe4,iVar3 / 0xe4,4,0);
      }
      iVar5 = iVar5 + 1;
      piVar4 = piVar4 + 1;
    } while (iVar5 < DAT_00eaaf64);
    DAT_00eaaf64 = 0;
    return;
  }
  DAT_00eaaf64 = 0;
  return;
}
