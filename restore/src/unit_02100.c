#include "restore_types.h"
#define DAT_005f5d08 (*(undefined4 *)0x005f5d08u)
#define DAT_007999db (*(undefined4 *)0x007999dbu)
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_00799a2c (*(undefined4 *)0x00799a2cu)
#define DAT_00bca2d0 (*(undefined *)0x00bca2d0u)
#define DAT_00bca2d4 (*(undefined *)0x00bca2d4u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00eaac28 (*(uint32_t *)0x00eaac28u)
#define DAT_00eaae8c (*(undefined *)0x00eaae8cu)
#define DAT_00eaae90 (*(undefined *)0x00eaae90u)
extern int FUN_0046a400();
extern void FUN_00470990();
extern void FUN_0047bae0();
extern bool FUN_004ecea0();
extern void FUN_004ed360();
extern void FUN_00516f90();
extern void FUN_005173a0();

void FUN_004eef10(int param_1,int param_2,undefined1 param_3)

{
  byte bVar1;
  char cVar2;
  int iVar3;
  int iVar4;
  uint uVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  bVar1 = *(byte *)(&DAT_00799a2c + param_1 * 0x84);
  cVar2 = FUN_004ecea0((uint)bVar1);
  if (cVar2 == '\0') {
    (&DAT_00bcd630)[param_2 * 0x184] = param_3;
    return;
  }
  iVar4 = (*(int *)(&DAT_005f5d08 + (uint)bVar1 * 4) + *(int *)(&DAT_007999e4 + param_1 * 0x108)) -
          DAT_00bca2d8;
  FUN_004ed360(iVar4 % 0xe4,iVar4 / 0xe4,3,0);
  iVar4 = DAT_00eaae90;
  param_2 = param_2 * 0x184;
  if ((DAT_00eaae90 == *(int *)(&DAT_00bcd620 + param_2)) && (DAT_00eaae90 != 0)) {
    *(short *)(&DAT_00bcd62e + param_2) = *(short *)(&DAT_00bcd62e + param_2) + 1;
    if (199 < *(short *)(&DAT_00bcd62e + param_2)) {
      iVar6 = (iVar4 - DAT_00bca2d8) % 0xe4;
      iVar4 = (iVar4 - DAT_00bca2d8) / 0xe4;
      iVar3 = FUN_0046a400(DAT_00eaac28,DAT_00eaae8c,iVar6,iVar4,0,0);
      FUN_00470990(DAT_00eaac28,DAT_00eaae8c,iVar3,iVar6,iVar4,2,1,0);
      FUN_0047bae0(0,0,DAT_00bca2d0 + -1,DAT_00bca2d4 + -1,0);
      iVar8 = iVar3 * 0x108;
      uVar5 = (uint)(byte)(&DAT_007999db)[iVar8];
      iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar8 + 2);
      FUN_00516f90(iVar7 - uVar5,(int)*(short *)(&DAT_007999e0 + iVar8) - uVar5,iVar7 + uVar5,
                   uVar5 + (int)*(short *)(&DAT_007999e0 + iVar8),1);
      iVar7 = (int)*(short *)((int)&DAT_007999dc + iVar8 + 2);
      uVar5 = (uint)(byte)(&DAT_007999db)[iVar8];
      FUN_005173a0(iVar7 - uVar5,(int)*(short *)(&DAT_007999e0 + iVar8) - uVar5,iVar7 + uVar5,
                   uVar5 + (int)*(short *)(&DAT_007999e0 + iVar8));
      (&DAT_00bcd630)[param_2] = param_3;
      FUN_00470990(DAT_00eaac28,DAT_00eaae8c,iVar3,iVar6,iVar4,2,0,0);
      return;
    }
  }
  else {
    (&DAT_00bcd630)[param_2] = param_3;
  }
  return;
}
