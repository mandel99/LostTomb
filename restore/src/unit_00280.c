#include "restore_types.h"
#define DAT_007028f8 (*(undefined *)0x007028f8u)
#define DAT_007028fc (*(undefined *)0x007028fcu)
#define DAT_00702904 (*(undefined4 *)0x00702904u)
#define DAT_0070290c (*(undefined4 *)0x0070290cu)
#define DAT_00702980 (*(undefined4 *)0x00702980u)
#define DAT_007029d0 (*(undefined4 *)0x007029d0u)
#define DAT_00e92e0c (*(undefined *)0x00e92e0cu)
extern undefined4 FUN_0047d350();

void FUN_0041d560(void)

{
  int *piVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  
  iVar5 = DAT_00e92e0c;
  piVar1 = &DAT_00702980;
  iVar4 = 1;
  do {
    piVar1[0x16] = 0;
    if (*piVar1 < 1) {
      piVar1[1] = 0;
      piVar1[3] = 0;
      piVar1[4] = 0;
      piVar1[5] = 0;
      piVar1[6] = 0;
      piVar1[7] = 0;
    }
    else {
      *piVar1 = 0;
      if ((piVar1[2] <= piVar1[1]) && (piVar1[0x15] <= iVar5 - piVar1[0x14])) {
        piVar1[0x16] = 1;
      }
    }
    piVar1 = piVar1 + 0x20;
  } while ((int)piVar1 < 0x704c00);
  if (1999 < iVar5 - DAT_007028fc) {
    iVar6 = 0;
    iVar3 = 0;
    piVar1 = &DAT_007029d0;
    do {
      if ((0 < piVar1[2]) && (iVar6 < iVar5 - *piVar1)) {
        iVar3 = iVar4;
        iVar6 = iVar5 - *piVar1;
      }
      piVar1 = piVar1 + 0x20;
      iVar4 = iVar4 + 1;
    } while ((int)piVar1 < 0x704c50);
    if (0 < iVar3) {
      iVar4 = 0;
      iVar5 = iVar3 * 0x80;
      do {
        iVar6 = *(int *)(iVar5 + 0x702920) + 1;
        *(int *)(iVar5 + 0x702920) = iVar6;
        if (*(int *)(iVar5 + 0x702924) <= iVar6) {
          *(undefined4 *)(iVar5 + 0x702920) = 0;
        }
        DAT_007028f8 = *(undefined4 *)((*(int *)(iVar5 + 0x702920) + iVar3 * 0x20) * 4 + 0x702928);
        iVar6 = FUN_0047d350(DAT_007028f8);
      } while ((iVar6 == 0) && (iVar4 = iVar4 + 1, iVar4 < 8));
      if (*(int *)(iVar5 + 0x702914) < 0xb) {
        if (*(int *)(&DAT_0070290c + iVar5) < 0xb) {
          cVar2 = ((*(int *)(iVar5 + 0x70291c) < 0xb) - 1U & 2) + 2;
        }
        else {
          cVar2 = '\0';
        }
      }
      else {
        cVar2 = '\x02';
      }
      FUN_0047d350(DAT_007028f8,cVar2);
      DAT_007028fc = DAT_00e92e0c;
      *(int *)(iVar5 + 0x702950) = DAT_00e92e0c;
      *(undefined4 *)(&DAT_00702904 + iVar5) = 0;
      *(undefined4 *)(&DAT_0070290c + iVar5) = 0;
      *(undefined4 *)(iVar5 + 0x702910) = 0;
      *(undefined4 *)(iVar5 + 0x702914) = 0;
      *(undefined4 *)(iVar5 + 0x702918) = 0;
      *(undefined4 *)(iVar5 + 0x70291c) = 0;
      *(int *)(iVar5 + 0x70294c) = *(int *)(iVar5 + 0x70294c) + 1;
    }
  }
  return;
}
