#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
extern void FUN_004a4850();
extern int FUN_004a5560();
extern void FUN_004ae270();
extern void FUN_004b3250();
extern undefined4 FUN_0051cba0();

void FUN_004ae490(int param_1,undefined4 param_2)

{
  int iVar1;
  undefined1 uVar2;
  int iVar3;
  int iVar4;
  undefined4 uVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  
  iVar1 = param_1;
  iVar4 = param_1 * 0x184;
  if (*(int *)(&DAT_00bcd754 + iVar4) == 0xb) {
    FUN_004ae270(param_1,param_2);
    if ((&DAT_00bcd609)[iVar4] == '\b') {
      *(undefined4 *)(&DAT_00bcd754 + iVar4) = 0x10;
    }
    return;
  }
  if (*(int *)(&DAT_00bcd754 + iVar4) != 0x10) {
    return;
  }
  *(short *)(&DAT_00bcd62e + iVar4) = *(short *)(&DAT_00bcd62e + iVar4) + 1;
  if (*(short *)(&DAT_00bcd62e + iVar4) < 0x4b) {
    return;
  }
  *(undefined2 *)(&DAT_00bcd62e + iVar4) = 0;
  iVar3 = FUN_004a5560(param_1,0x12,1);
  if (iVar3 == 0) {
    (&DAT_00bcd630)[iVar4] = 0x11;
    return;
  }
  uVar2 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[iVar3 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar3 * 0xc2],
                       (int)(short)(&DAT_00bcd60c)[iVar1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar1 * 0xc2]);
  switch((int)(uVar2)) {
  case 0: ;
    uVar10 = 0xfffffff6;
    uVar9 = 0xfffffff6;
    uVar8 = 0xfffffff6;
    uVar7 = 10;
    uVar6 = 0xfffffff6;
    uVar5 = 0;
    break;
  case 1: ;
    uVar10 = 0;
    uVar9 = 10;
    uVar8 = 0xfffffff6;
    uVar7 = 10;
    uVar6 = 0xfffffff6;
    uVar5 = 0;
    break;
  case 2: ;
    uVar9 = 10;
    uVar7 = 10;
    uVar6 = 0xfffffff6;
    uVar5 = 10;
    goto code_r0x004ae58c;
  case 3: ;
    uVar10 = 10;
    uVar9 = 0;
    uVar8 = 10;
    uVar7 = 10;
    uVar6 = 0;
    uVar5 = 10;
    break;
  case 4: ;
    uVar10 = 10;
    uVar9 = 0xfffffff6;
    uVar8 = 10;
    uVar7 = 0;
    uVar6 = 10;
    uVar5 = 10;
    break;
  case 5: ;
    uVar9 = 0xfffffff6;
    uVar7 = 0xfffffff6;
    uVar6 = 10;
    uVar5 = 0;
code_r0x004ae58c: ;
    uVar10 = 10;
    uVar8 = 0;
    break;
  case 6: ;
    uVar10 = 0xfffffff6;
    uVar9 = 0xfffffff6;
    uVar8 = 10;
    uVar7 = 0xfffffff6;
    uVar6 = 0;
    uVar5 = 0xfffffff6;
    break;
default: ;
    uVar10 = 0xfffffff6;
    uVar9 = 0;
    uVar8 = 0xfffffff6;
    uVar7 = 0xfffffff6;
    uVar6 = 0;
    uVar5 = 0xfffffff6;
  }
  FUN_004a4850(&param_1,&param_2,uVar5,uVar6,uVar7,uVar8,uVar9,uVar10,iVar1);
  *(undefined2 *)(&DAT_00bcd61c + iVar4) = (undefined2)param_1;
  *(undefined2 *)(&DAT_00bcd61e + iVar4) = (undefined2)param_2;
  FUN_004b3250(iVar1);
  *(undefined4 *)(&DAT_00bcd754 + iVar4) = 0xb;
  return;
}
