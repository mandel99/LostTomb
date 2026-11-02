#include "restore_types.h"
#define DAT_00bcd609 (*(undefined4 *)0x00bcd609u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd62e (*(undefined4 *)0x00bcd62eu)
#define DAT_00bcd754 (*(undefined4 *)0x00bcd754u)
extern void FUN_004a4850();
extern int FUN_004a5560();
extern void FUN_004a6890();
extern void FUN_004b3250();
extern undefined4 FUN_0051cba0();

void FUN_004a6970(int param_1,undefined4 *param_2)

{
  int iVar1;
  undefined4 *puVar2;
  undefined1 uVar3;
  int iVar4;
  int iVar5;
  undefined4 uVar6;
  undefined4 uVar7;
  undefined4 uVar8;
  undefined4 uVar9;
  undefined4 uVar10;
  undefined4 uVar11;
  
  puVar2 = param_2;
  iVar1 = param_1;
  iVar5 = param_1 * 0x184;
  if (*(int *)(&DAT_00bcd754 + iVar5) == 0xb) {
    FUN_004a6890(param_1,param_2);
    if ((&DAT_00bcd609)[iVar5] == '\b') {
      *(undefined4 *)(&DAT_00bcd754 + iVar5) = 0x10;
      *puVar2 = 0;
    }
    return;
  }
  if (*(int *)(&DAT_00bcd754 + iVar5) != 0x10) {
    return;
  }
  *param_2 = 0;
  *(short *)(&DAT_00bcd62e + iVar5) = *(short *)(&DAT_00bcd62e + iVar5) + 1;
  if (*(short *)(&DAT_00bcd62e + iVar5) < 0x4b) {
    return;
  }
  *(undefined2 *)(&DAT_00bcd62e + iVar5) = 0;
  iVar4 = FUN_004a5560(param_1,0x12,0);
  if (iVar4 == 0) {
    return;
  }
  uVar3 = FUN_0051cba0((int)(short)(&DAT_00bcd60c)[iVar4 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar4 * 0xc2],
                       (int)(short)(&DAT_00bcd60c)[iVar1 * 0xc2],
                       (int)(short)(&DAT_00bcd60e)[iVar1 * 0xc2]);
  switch((int)(uVar3)) {
  case 0: ;
    uVar11 = 0xfffffff6;
    uVar10 = 0xfffffff6;
    uVar9 = 0xfffffff6;
    uVar8 = 10;
    uVar7 = 0xfffffff6;
    uVar6 = 0;
    break;
  case 1: ;
    uVar11 = 0;
    uVar10 = 10;
    uVar9 = 0xfffffff6;
    uVar8 = 10;
    uVar7 = 0xfffffff6;
    uVar6 = 0;
    break;
  case 2: ;
    uVar10 = 10;
    uVar8 = 10;
    uVar7 = 0xfffffff6;
    uVar6 = 10;
    goto LAB_004a6a77;
  case 3: ;
    uVar11 = 10;
    uVar10 = 0;
    uVar9 = 10;
    uVar8 = 10;
    uVar7 = 0;
    uVar6 = 10;
    break;
  case 4: ;
    uVar11 = 10;
    uVar10 = 0xfffffff6;
    uVar9 = 10;
    uVar8 = 0;
    uVar7 = 10;
    uVar6 = 10;
    break;
  case 5: ;
    uVar10 = 0xfffffff6;
    uVar8 = 0xfffffff6;
    uVar7 = 10;
    uVar6 = 0;
LAB_004a6a77: ;
    uVar11 = 10;
    uVar9 = 0;
    break;
  case 6: ;
    uVar11 = 0xfffffff6;
    uVar10 = 0xfffffff6;
    uVar9 = 10;
    uVar8 = 0xfffffff6;
    uVar7 = 0;
    uVar6 = 0xfffffff6;
    break;
default: ;
    uVar11 = 0xfffffff6;
    uVar10 = 0;
    uVar9 = 0xfffffff6;
    uVar8 = 0xfffffff6;
    uVar7 = 0;
    uVar6 = 0xfffffff6;
  }
  FUN_004a4850(&param_2,&param_1,uVar6,uVar7,uVar8,uVar9,uVar10,uVar11,iVar1);
  *(undefined2 *)(&DAT_00bcd61c + iVar5) = RESTORE_SLICE2(param_2,0);
  *(undefined2 *)(&DAT_00bcd61e + iVar5) = (undefined2)param_1;
  FUN_004b3250(iVar1);
  *(undefined4 *)(&DAT_00bcd754 + iVar5) = 0xb;
  return;
}
