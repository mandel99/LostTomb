#include "restore_types.h"
#define DAT_007999dc (*(undefined4 *)0x007999dcu)
#define DAT_007999e0 (*(undefined4 *)0x007999e0u)
#define DAT_007999e4 (*(undefined4 *)0x007999e4u)
#define DAT_007999ea (*(undefined4 *)0x007999eau)
#define DAT_007999fe (*(undefined4 *)0x007999feu)
#define DAT_00799a00 (*(undefined4 *)0x00799a00u)
#define DAT_00799a34 (*(undefined4 *)0x00799a34u)
#define DAT_00799a40 (*(undefined4 *)0x00799a40u)
#define DAT_00799a44 (*(undefined4 *)0x00799a44u)
#define DAT_00799a48 (*(undefined4 *)0x00799a48u)
#define DAT_00bca2d8 (*(undefined *)0x00bca2d8u)
#define DAT_00bca2e0 (*(undefined *)0x00bca2e0u)
#define DAT_00bcd602 (*(undefined4 *)0x00bcd602u)
#define DAT_00bcd60c (*(undefined4 *)0x00bcd60cu)
#define DAT_00bcd60e (*(undefined4 *)0x00bcd60eu)
#define DAT_00bcd618 (*(undefined4 *)0x00bcd618u)
#define DAT_00bcd61c (*(undefined4 *)0x00bcd61cu)
#define DAT_00bcd61e (*(undefined4 *)0x00bcd61eu)
#define DAT_00bcd620 (*(undefined4 *)0x00bcd620u)
#define DAT_00bcd630 (*(undefined4 *)0x00bcd630u)
#define DAT_00bcd63c (*(undefined4 *)0x00bcd63cu)
#define DAT_00bcd660 (*(undefined4 *)0x00bcd660u)
#define DAT_00d056f8 (*(undefined *)0x00d056f8u)
extern void FUN_004b3250();
extern void FUN_004b52f0();
extern void FUN_004b5420();
extern void FUN_004ded50();
extern void FUN_004dfbb0();
extern undefined4 FUN_004e0c50();

void FUN_004dfd90(int param_1)

{
  char cVar1;
  undefined4 uVar2;
  int iVar3;
  undefined2 uVar4;
  int iVar5;
  int iVar6;
  int iVar7;
  int local_38;
  int local_34;
  int local_30;
  
  iVar3 = param_1;
  iVar6 = param_1 * 0x108;
  FUN_004ded50(&local_34,(int)(short)(&DAT_007999ea)[param_1 * 0x84],DAT_00bca2e0,
               *(undefined4 *)(&DAT_00799a48 + iVar6));
  local_38 = *(int *)(&DAT_00799a44 + iVar3 * 0x84) + local_30;
  param_1 = *(int *)(&DAT_00799a40 + iVar3 * 0x84) + local_34;
  *(short *)((int)&DAT_007999dc + iVar6 + 2) = (short)param_1;
  *(short *)(&DAT_007999e0 + iVar6) = (short)local_38;
  *(int *)(&DAT_007999e4 + iVar6) = DAT_00bca2d8 + local_38 * 0xe4 + param_1;
  FUN_004e0c50(iVar3,&param_1,&local_38);
  (&DAT_007999fe)[iVar3 * 0x84] = (undefined2)param_1;
  uVar4 = (undefined2)local_38;
  (&DAT_00799a00)[iVar3 * 0x84] = uVar4;
  iVar5 = *(int *)(&DAT_00799a34 + iVar6);
  if (iVar5 == 0) goto LAB_004e003b;
  iVar7 = iVar5 * 0x184;
  cVar1 = (&DAT_00bcd602)[iVar7];
  if (cVar1 == 'O') {
    if ((&DAT_00bcd630)[iVar7] == '\t') {
      *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_007999fe)[iVar3 * 0x84];
      *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar4;
      *(undefined4 *)(&DAT_00bcd620 + iVar7) = *(undefined4 *)(&DAT_007999e4 + iVar6);
      goto LAB_004dfee3;
    }
    if ((&DAT_00bcd630)[iVar7] != '\n') goto LAB_004e003b;
    FUN_004b5420(iVar5);
    uVar4 = *(undefined2 *)(&DAT_007999e0 + iVar6);
    uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar6);
    (&DAT_00bcd60c)[iVar5 * 0xc2] = *(undefined2 *)((int)&DAT_007999dc + iVar6 + 2);
    (&DAT_00bcd60e)[iVar5 * 0xc2] = uVar4;
    *(undefined4 *)(&DAT_00bcd618 + iVar7) = uVar2;
LAB_004e0032: ;
    FUN_004b52f0(iVar5);
LAB_004e003b: ;
    FUN_004dfbb0(iVar3);
    return;
  }
  if (cVar1 == 'Q') {
    cVar1 = (&DAT_00bcd630)[iVar7];
    if (cVar1 != '\t') {
      if ((cVar1 == '\v') || (cVar1 == '\n')) {
        FUN_004b5420(iVar5);
        uVar4 = *(undefined2 *)(&DAT_007999e0 + iVar6);
        (&DAT_00bcd60c)[iVar5 * 0xc2] = *(undefined2 *)((int)&DAT_007999dc + iVar6 + 2);
        uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar6);
        (&DAT_00bcd60e)[iVar5 * 0xc2] = uVar4;
        *(undefined4 *)(&DAT_00bcd618 + iVar7) = uVar2;
        *(undefined4 *)(&DAT_00bcd620 + iVar7) = uVar2;
        FUN_004b52f0(iVar5);
        if (((&DAT_00bcd630)[iVar7] == '\n') &&
           (((iVar5 = (int)*(short *)(&DAT_00bcd660 + DAT_00d056f8 * 0x184), iVar5 != 0 &&
             (iVar7 = iVar5 * 0x184, (&DAT_00bcd602)[iVar7] == 'U')) &&
            ((&DAT_00bcd630)[iVar7] == '\x1a')))) {
          uVar4 = (&DAT_00799a00)[iVar3 * 0x84];
          *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_007999fe)[iVar3 * 0x84];
          uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar6);
          *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar4;
          *(undefined4 *)(&DAT_00bcd620 + iVar7) = uVar2;
          goto LAB_004dfee3;
        }
      }
      goto LAB_004e003b;
    }
  }
  else {
    if (cVar1 != 'U') goto LAB_004e003b;
    if ((&DAT_00bcd630)[iVar7] != '\x1b') {
      if ((&DAT_00bcd630)[iVar7] != '\x1c') goto LAB_004e003b;
      FUN_004b5420(iVar5);
      uVar4 = *(undefined2 *)(&DAT_007999e0 + iVar6);
      uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar6);
      (&DAT_00bcd60c)[iVar5 * 0xc2] = *(undefined2 *)((int)&DAT_007999dc + iVar6 + 2);
      (&DAT_00bcd60e)[iVar5 * 0xc2] = uVar4;
      *(undefined4 *)(&DAT_00bcd618 + iVar7) = uVar2;
      goto LAB_004e0032;
    }
  }
  uVar2 = *(undefined4 *)(&DAT_007999e4 + iVar6);
  *(undefined2 *)(&DAT_00bcd61c + iVar7) = (&DAT_007999fe)[iVar3 * 0x84];
  *(undefined2 *)(&DAT_00bcd61e + iVar7) = uVar4;
  *(undefined4 *)(&DAT_00bcd620 + iVar7) = uVar2;
LAB_004dfee3: ;
  FUN_004b3250(iVar5);
  *(undefined2 *)(&DAT_00bcd63c + iVar7) = 0;
  FUN_004dfbb0(iVar3);
  return;
}
