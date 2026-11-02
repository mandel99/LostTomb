#include "restore_types.h"
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999e8 (*(undefined *)0x007999e8u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00eaae9c (*(undefined *)0x00eaae9cu)
extern void FUN_004746d0();
extern void FUN_00475cc0();
extern void FUN_004767d0();
extern void FUN_00477500();
extern void FUN_004776f0();
extern void FUN_004777c0();
extern void FUN_00477890();
extern bool FUN_004ecea0();
extern void FUN_004ed360();

void FUN_004f4930(int param_1)

{
  byte bVar1;
  short sVar2;
  char cVar3;
  int iVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  if ((&DAT_007999e8)[param_1 * 0x84] == 0xb7) {
    sVar2 = (&DAT_007999ea)[param_1 * 0x84];
    bVar1 = *(byte *)(&DAT_00799a2c + (param_1 - sVar2) * 0x84);
    cVar3 = FUN_004ecea0((uint)bVar1);
    if (cVar3 != '\0') {
      iVar4 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) +
              *(int *)(&DAT_007999e4 + (param_1 - sVar2) * 0x108)) - DAT_00bca2d8;
      iVar5 = iVar4 % 0xe4;
      iVar4 = iVar4 / 0xe4;
      FUN_004ed360(iVar5,iVar4,5,0);
      iVar7 = ((DAT_00eaae9c - DAT_00bca2d8) % 0xe4 - iVar5) + 2;
      iVar8 = iVar7 + iVar5;
      iVar6 = (DAT_00eaae9c - DAT_00bca2d8) / 0xe4 + 2;
      FUN_004767d0(iVar5,iVar4,iVar8,iVar6,0,0);
      FUN_004746d0(iVar5,iVar4,iVar7);
      FUN_00477500(iVar5,iVar4,iVar8,iVar6);
      FUN_004777c0(iVar5,iVar4,iVar8,iVar6);
      FUN_00475cc0(iVar5,iVar4,iVar8,iVar6,0);
      FUN_004776f0(iVar5,iVar4,iVar8,iVar6);
      FUN_00477890(iVar5,iVar4,iVar8,iVar6);
    }
  }
  return;
}
