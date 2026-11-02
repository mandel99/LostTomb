#include "restore_types.h"
#define DAT_005dc24c (*(undefined4 *)0x005dc24cu)
#define DAT_007090f4 (*(undefined4 *)0x007090f4u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60a (*(undefined4 *)0x00bcd60au)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd67a (*(undefined4 *)0x00bcd67au)
#define DAT_00bcd720 (*(undefined4 *)0x00bcd720u)
#define DAT_00bcd72c (*(undefined4 *)0x00bcd72cu)
#define DAT_00bcd748 (*(undefined4 *)0x00bcd748u)
#define DAT_00bcd750 (*(undefined4 *)0x00bcd750u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
#define DAT_00bcd75c (*(undefined4 *)0x00bcd75cu)
#define DAT_00d3b374 (*(undefined *)0x00d3b374u)
extern void FUN_0048c0e0();
extern void FUN_004a5680();
extern void FUN_004a5880();
extern int FUN_004a5c10();
extern void FUN_004b3250();

void FUN_004ae2c0(int param_1,undefined4 *param_2)

{
  char cVar1;
  byte bVar2;
  int iVar3;
  uint uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int iVar8;
  
  iVar6 = 0;
  FUN_004a5680(param_1,1);
  *param_2 = 1;
  iVar7 = param_1 * 0x184;
  if (*(int *)(&DAT_00bcd75c + iVar7) != 0) {
    uVar4 = *(int *)(&DAT_00bcd75c + iVar7) - 1;
    if (((int)uVar4 < 0) || ((int)uVar4 < 0xf)) {
      uVar4 = uVar4 & ((int)uVar4 < 0) - 1;
    }
    else {
      uVar4 = 0xf;
    }
    *(uint *)(&DAT_00bcd75c + iVar7) = uVar4;
    goto LAB_004ae454;
  }
  iVar3 = FUN_004a5c10(param_1);
  if (iVar3 == 0) goto LAB_004ae454;
  iVar8 = iVar3 * 0x184;
  if (((&DAT_00bcd602)[iVar8] == 'N') && ((&DAT_00bcd630)[iVar8] == '\x14')) {
    iVar6 = 0x14;
  }
  cVar1 = (&DAT_00bcd609)[iVar8];
  if ('\a' < cVar1) {
    cVar1 = (&DAT_00bcd60a)[iVar8];
  }
  bVar2 = (&DAT_005dc24c)[cVar1 * 8 + (int)(char)(&DAT_00bcd609)[iVar7]];
  if (*(int *)(&DAT_00bcd72c + iVar7) <
      *(int *)(&DAT_007090f4 + (uint)(byte)(&DAT_00bcd602)[iVar7] * 0x38)) {
    iVar5 = *(int *)(&DAT_00bcd750 + iVar7);
  }
  else {
    iVar5 = *(int *)(&DAT_00bcd748 + iVar7);
  }
  FUN_004a5880((uint)bVar2);
  *(int *)(&DAT_00bcd720 + iVar8) =
       *(int *)(&DAT_00bcd720 + iVar8) - (iVar5 - iVar6) / (int)(uint)bVar2;
  FUN_004b3250(param_1);
  iVar6 = (DAT_00d3b374 & 2) - 1;
  iVar5 = (char)(&DAT_00bcd609)[iVar7] + iVar6;
  if (iVar5 < 7) {
    if (iVar5 < 0) {
      iVar5 = 0;
    }
    else if (6 < iVar5) goto LAB_004ae3c9;
  }
  else {
LAB_004ae3c9: ;
    iVar5 = 7;
  }
  iVar6 = (char)(&DAT_00bcd609)[iVar8] - iVar6;
  if (iVar6 < 7) {
    if (iVar6 < 0) {
      iVar6 = 0;
    }
    else if (6 < iVar6) goto LAB_004ae3e9;
  }
  else {
LAB_004ae3e9: ;
    iVar6 = 7;
  }
  (&DAT_00bcd609)[iVar7] = (char)iVar5;
  (&DAT_00bcd609)[iVar8] = (char)iVar6;
  *(undefined4 *)(&DAT_00bcd75c + iVar7) = 0xf;
  (&DAT_00bcd630)[iVar7] = 10;
  *(undefined2 *)(&DAT_00bcd67a + iVar7) = 0;
  FUN_0048c0e0((int)(short)(&DAT_00bcd60c)[iVar3 * 0xc2],(int)(short)(&DAT_00bcd60e)[iVar3 * 0xc2],0
               ,0);
LAB_004ae454: ;
  cVar1 = (&DAT_00bcd609)[iVar7];
  if (cVar1 == '\b') {
    (&DAT_00bcd630)[iVar7] = 10;
    *(undefined4 *)(&DAT_00bcd754 + iVar7) = 0x10;
    return;
  }
  if (cVar1 != '\t') {
    if (cVar1 != '\n') {
      return;
    }
    (&DAT_00bcd630)[iVar7] = 4;
  }
  FUN_004b3250(param_1);
  return;
}
